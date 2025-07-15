#include <bits/stdc++.h>
using namespace std;

class Train
{
protected:
    string train_no;
    string train_name;
    // int track_no;
    int arrival_time;
    int departure_time;
    string source_station;
    string destination_station;

public:
    Train(string train_no, string train_name, int arrival_time, int departure_time, string source, string destination)
    {
        this->train_no = train_no;
        this->train_name = train_name;
        // this->track_no = track_no
        this->arrival_time = arrival_time;
        this->departure_time = departure_time;
        this->source_station = source;
        this->destination_station = destination;
    }

    // setters
    void change_train_name(string train_name)
    {
        this->train_name = train_name;
        cout << "Train name changed to" << train_name << endl;
    }

    void change_train_arrival(int arrival)
    {
        this->arrival_time = arrival;
        cout << "changed the arrival time of train" << train_name << " to " << arrival_time << endl;
    }
    void change_train_departure(int departure)
    {
        this->departure_time = departure;
        cout << "changed the departure time of train" << train_name << " to " << departure_time << endl;
    }

    // void change_track(int new_track){
    //     this->track_no = new_track;
    // }

    // getters
    string get_train_no()
    {
        return train_no;
    }

    string get_train_name()
    {
        return train_name;
    }
    int get_train_arrival_time()
    {
        return arrival_time;
    }
    int get_train_departure_time()
    {
        return departure_time;
    }
    string get_train_source()
    {
        return source_station;
    }
    string get_train_destination()
    {
        return destination_station;
    }
    // int get_track_no(){
    //     return track_no;
    // }
};

class Station
{
protected:
    string station_id;
    string station_name;
    int no_of_tracks;
    vector<Train *> trains;
    unordered_map<string, int> track_allocation;
    unordered_map<string, int> train_delay;
    unordered_map<string, int> alloted_delay;

    int changeToHour(int time)
    {
        int hour = time / 60;
        int min = time % 60;
        return hour * 100 + min;
    }

    int changeToMinute(int time)
    {
        int hour = time / 100;
        int minute = time % 100;
        return hour * 60 + time;
    }

    void reallocate_tracks()
    {
        // Collect all trains with their adjusted times
        vector<pair<Train *, pair<int, int>>> all_trains;

        for (auto train : trains)
        {
            string train_id = train->get_train_no();
            int arrival = train->get_train_arrival_time();
            int departure = train->get_train_departure_time();

            if (train_delay.find(train_id) != train_delay.end())
            {
                arrival += train_delay[train_id];
                departure += train_delay[train_id];
            }
            all_trains.push_back({train, {arrival, departure}});
        }

        // Sort trains by arrival time, then by departure time (fixed comparator)
        sort(all_trains.begin(), all_trains.end(), [](const auto &a, const auto &b)
             {
        if (a.second.first == b.second.first) {
            return a.second.second < b.second.second;
        }
        return a.second.first < b.second.first; });

        // Priority queue for available tracks (min-heap)
        priority_queue<int, vector<int>, greater<int>> available_tracks;

        // Priority queue for track availability times: {time, track_id}
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> next_availability;

        // Initialize all tracks as available
        for (int i = 1; i <= no_of_tracks; i++)
        {
            available_tracks.push(i);
        }

        for (const auto &train_info : all_trains)
        {
            int arrival = train_info.second.first;
            int departure = train_info.second.second;
            string train_no = train_info.first->get_train_no();

            // Free up tracks that have become available by this train's arrival time
            while (!next_availability.empty() && next_availability.top().first <= arrival)
            {
                available_tracks.push(next_availability.top().second);
                next_availability.pop();
            }

            int actual_start_time = arrival;

            // If no tracks available, wait for the next one to become free
            if (available_tracks.empty())
            {
                actual_start_time = next_availability.top().first;

                // Free up all tracks that become available at this time
                while (!next_availability.empty() && next_availability.top().first <= actual_start_time)
                {
                    available_tracks.push(next_availability.top().second);
                    next_availability.pop();
                }
            }

            // Assign the lowest numbered available track
            int assigned_track = available_tracks.top();
            available_tracks.pop();

            // Record the allocation
            this->track_allocation[train_no] = assigned_track;
            this->alloted_delay[train_no] = actual_start_time - arrival;

            // Calculate when this track will be free again
            int track_free_time = departure + (actual_start_time - arrival);
            next_availability.push({track_free_time, assigned_track});
        }
    }

    vector<Train *> search_trains(vector<Train *> trains, int time)
    {
        int low = 0;
        int high = trains.size() - 1;
        while (low < high)
        {
            int mid = low + (high - low) / 2;
            if (trains[mid]->get_train_arrival_time() < time)
            {
                low = mid + 1;
            }
            else
            {
                high = mid;
            }
        }
        vector<Train *> next_trains(trains.begin() + low, trains.end());
        return next_trains;
    }

    void print_trains(vector<Train *> trains, int n = 0)
    {
        n = (trains.size() + n - 1) % trains.size();
        for (int i = 0; i <= n; i++)
        {
            cout << "Train : " << trains[i]->get_train_name() << "( ID:  " << trains[i]->get_train_no() << " )" << endl;
            cout << "Route : " << trains[i]->get_train_source() << " -> " << trains[i]->get_train_destination() << endl;
            cout << "Arrival: " << changeToHour(trains[i]->get_train_arrival_time()) << "    Departure:" << changeToHour(trains[i]->get_train_departure_time()) << "    Track:" << track_allocation[trains[i]->get_train_no()] << endl;
            cout << "Delay: " << alloted_delay[trains[i]->get_train_no()] << " minutes" << endl;
        }
    }

public:
    Station(string station_id, string station_name, int no_of_tracks)
    {
        this->station_id = station_id;
        this->station_name = station_name;
        this->no_of_tracks = no_of_tracks;
        cout << "Station Created" << endl;
    }

    ~Station()
    {
        for (auto train : trains)
        {
            delete train;
        }
        cout << "Deleted all trains and station" << endl;
    }

    void get_station_details()
    {
        cout << "Staion ID: " << station_id << endl;
        cout << "Station Name: " << station_name << endl;
        cout << "No of tracks: " << no_of_tracks << endl;
        cout << "No of trains: " << trains.size() << endl;
    }

    int get_tracks()
    {
        return no_of_tracks;
    }

    int get_no_of_trains()
    {
        return trains.size();
    }

    string get_id()
    {
        return station_id;
    }

    string get_name()
    {
        return station_name;
    }

    void change_no_of_tracks(int tracks)
    {
        this->no_of_tracks = tracks;
        reallocate_tracks();
    }

    void change_name(string new_name)
    {
        this->station_name = new_name;
    }

    void add_train(string train_name, int arrival, int departure, string source, string destination)
    {

        string train_no = "TN-" + to_string(trains.size() + 1);
        int arrival_time = changeToMinute(arrival);
        int departure_time = changeToMinute(departure);
        // calculation for track allotment

        Train *new_train = new Train(train_no, train_name, arrival_time, departure_time, source, destination);
        trains.push_back(new_train);
        cout << "Added train" << endl;

        reallocate_tracks();
    }

    void delete_train(string train_id)
    {
        auto it = trains.begin();
        while (it != trains.end())
        {
            if ((*it)->get_train_no() == train_id)
                break;
        }
        if (it == trains.end())
        {
            cout << "INVALID TRAIN ID" << endl;
            return;
        }
        trains.erase(it);
        cout << "Train Deleted Successfully" << endl;
        reallocate_tracks();
    }

    // overload for n train on any tracks
    void get_all_train(string sortby = "")
    {
        vector<Train *> trains = this->trains;
        if (sortby == "track_no")
        {
            sort(trains.begin(), trains.end(), [&](Train *a, Train *b)
                 { return track_allocation[a->get_train_no()] <= track_allocation[b->get_train_no()]; });
        }
        else if (sortby == "arrival_time")
        {
            sort(trains.begin(), trains.end(), [](Train *a, Train *b)
                 { return a->get_train_arrival_time() <= b->get_train_arrival_time(); });
        }
        else if (sortby == "departure_time")
        {
            sort(trains.begin(), trains.end(), [](Train *a, Train *b)
                 { return a->get_train_departure_time() <= b->get_train_departure_time(); });
        }
        cout << "Print called" << endl;
        // print the trains
        print_trains(trains);
    }

    // add delay to one train
    void add_delay(string train_id, int delay)
    {
        alloted_delay[train_id] = delay;
        reallocate_tracks();
    }
    void get_next_trains(int time)
    {
        sort(trains.begin(), trains.end(), [](Train *a, Train *b)
             { return a->get_train_arrival_time() <= b->get_train_arrival_time(); });
        vector<Train *> next_trains;
        next_trains = search_trains(trains, changeToMinute(time));
        print_trains(next_trains);
    }

    void get_next_trains(int time, int n)
    {
        sort(trains.begin(), trains.end(), [](Train *a, Train *b)
             { return (a->get_train_arrival_time() <= b->get_train_arrival_time()); });
        vector<Train *> next_trains;
        next_trains = search_trains(trains, changeToMinute(time));
        if (n > next_trains.size())
            n = next_trains.size();
        print_trains(next_trains, n);
    }

    void get_next_trains(int time, int n, int track_no)
    {
        vector<Train *> trains_acc_track;
        for (auto train : trains)
        {
            if (track_allocation[train->get_train_no()] == track_no)
            {
                trains_acc_track.push_back(train);
            }
        }
        sort(trains_acc_track.begin(), trains_acc_track.end(), [](Train *a, Train *b)
             { return (a->get_train_arrival_time() <= b->get_train_arrival_time()); });
        trains_acc_track = search_trains(trains_acc_track, changeToMinute(time));
        if (n > trains_acc_track.size() || n == -1)
            n = trains_acc_track.size();
        print_trains(trains_acc_track, n);
    }
};

int main()
{
    cout << "WELCOME TO STATION MANAGEMENT APP" << endl;
    while (true)
    {
        char choice;
        cout << "1. Create a new Station" << endl;
        cout << "CHOOSE From below Functionalities (q to quit): " << endl;
        cin >> choice;
        if (tolower(choice) == 'q')
            break;
        if (choice == '1')
        {
            string station_name;
            int no_of_tracks;
            cout << "Enter station Name: ";
            cin >> station_name;
            cout << "Enter the number of tracks: ";
            cin >> no_of_tracks;
            Station *station = new Station("STN1", station_name, no_of_tracks);
            // cout<<"Station Created-----"<<endl;

            while (true)
            {
                cout << "1. Add/Delete trains" << endl;
                cout << "2. View Next trains" << endl;
                cout << "3. Add delay" << endl;
                cout << "4. View all trains" << endl;
                cout << "5. Get station details" << endl;
                cout << "CHOOSE From above Functionalities (q to quit): ";
                cin >> choice;
                if (tolower(choice) == 'q')
                    break;

                else if (choice == '1')
                {
                    cout << "1. Add train" << endl;
                    cout << "2. Delete train" << endl;
                    cout << "CHOOSE From above Functionalities (q to quit): ";
                    cin >> choice;
                    if (choice == '1')
                    {
                        string train_name, source, destination;
                        int arrival, departure;

                        cout << "Enter train Name: ";
                        cin >> train_name;
                        cout << "Enter Source Name: ";
                        cin >> source;
                        cout << "Enter Destination Name: ";
                        cin >> destination;
                        cout << "Enter Arrival Name: ";
                        cin >> arrival;
                        cout << "Enter Departure Name: ";
                        cin >> departure;
                        if (arrival < 0 || arrival >= 2400 || departure < 0 || departure >= 2400)
                        {
                            cout << "Wrong timing--- Try Again" << endl;
                        }
                        else
                        {
                            station->add_train(train_name, arrival, departure, source, destination);
                            cout << "Train Added" << endl;
                        }
                    }
                    else if (choice == '2')
                    {
                        station->get_all_train();
                        string train_id;
                        cout << "Enter train ID";
                        cin >> train_id;
                        station->delete_train(train_id);
                    }
                }
                else if (choice == '2')
                {
                    int time_now;
                    int track_no;
                    int no_of_trains;

                    cout << "Enter Time:";
                    cin >> time_now;
                    cout << "Enter No of trains (-1 : for all trains)";
                    cin >> no_of_trains;
                    cout << "Enter track no(-1 : for all tracks) Available tracks(1 - " << station->get_tracks() << "): ";
                    cin >> track_no;
                    if (time_now < 0 || time_now >= 2400)
                    {
                        cout << "Wrong Time -- Try Again" << endl;
                    }
                    else if (no_of_trains == -1 && track_no == -1)
                    {
                        station->get_next_trains(time_now);
                    }

                    else if (track_no == -1 && no_of_trains > 0)
                    {
                        station->get_next_trains(time_now, no_of_trains);
                    }
                    else if (track_no > 0)
                    {
                        station->get_next_trains(time_now, no_of_trains, track_no);
                    }
                }

                else if (choice == '3')
                {
                    string train_id;
                    int delay;
                    station->get_all_train();
                    cout << "Enter the train ID: ";
                    cin >> train_id;
                    cout << "Enter Delay (in minutes) : ";
                    cin >> delay;
                    station->add_delay(train_id, delay);
                }
                else if (choice == '4')
                {
                    cout << "ALL Trains List: " << endl;
                    ;
                    station->get_all_train();
                }
                else if (choice == '5')
                {
                    station->get_station_details();
                }
            }
        }
    }
    cout << "---------Exited----------" << endl;
    return 0;
}
