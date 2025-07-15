# 🚉 Station Management System

A simple C++ console-based application to manage train schedules and track allocations at a station. This project simulates how trains arrive and depart from tracks and ensures optimal allocation using a priority queue and reallocation logic when delays are introduced.

## ✨ Features

- Add and delete trains
- Allocate tracks optimally based on arrival/departure and availability
- View upcoming trains (with options for time, count, and specific tracks)
- Introduce delays and automatically reallocate tracks
- Sort and view all trains by arrival, departure, or track
- Real-time console interaction

## 🧠 Core Concepts Used

- Object-Oriented Programming (OOP)
- Priority Queues (Min-heaps)
- Sorting and Binary Search
- Dynamic memory management
- Encapsulation and modular design

## 🛠️ Installation & Running

1. **Clone the repository**
   ```bash
   git clone https://github.com/Akataruka/station-management-app.git
   cd station-management-app


2. **Compile the code**

   ```bash
   g++ -std=c++17 main.cpp -o station-management-app
   ```

3. **Run the app**

   ```bash
   ./station-management-app
   ```

## 📦 File Structure

```
📁 station-management-system
├── main.cpp        # All classes and logic for train/station management
├── README.md       # Project information and usage
```

## 🚀 Example

```txt
WELCOME TO STATION MANAGEMENT APP
1. Create a new Station
CHOOSE From below Functionalities (q to quit):
Enter station Name: Central
Enter the number of tracks: 3
Station Created
1. Add/Delete trains
2. View Next trains
3. Add delay
4. View all trains
5. Get station details
...
```

## 📋 TODO

* Add file-based persistence for station/train data
* Add support for train ID search and editing

## 🤝 Contributing

Feel free to fork this repo and create a pull request for any improvements, bug fixes, or new features. PRs are welcome!

## 📝 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## 🙋‍♂️ Author

* **Asutosh Kataruka**
  3rd Year Undergraduate, IIIT Bhubaneswar
  Passionate about C++, LLD, ML-DL etc.

---

> ⭐ If you found this helpful, feel free to star the repo and share!

