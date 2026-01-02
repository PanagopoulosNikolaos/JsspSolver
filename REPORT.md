# JSSP Solver - Project Showcase

A C++ application designed to solve the **Job Shop Scheduling Problem (JSSP)** using basic algorithms and providing visual feedback via Gantt charts.

## User Interface Overview

### 1. Problem Selection
The application features a clean interface where users can select from pre-defined JSSP problems or load their own.

![Main Window](images/image.png)
*The main dashboard upon startup.*

![Problem Selection](<images/image copy.png>)
*Dropdown menu for selecting different problem instances (e.g., 5x5, 8x8, 12x4).*

---

### 2. Solving the Problem
Users can choose an algorithm to find an optimal or near-optimal schedule.

![Solving](<images/image copy 2.png>)
*Selecting an algorithm and executing the solver. The logs show the progress and the calculated Makespan.*

---

### 3. Visualization & Gantt Charts
The tool provides an integrated Gantt chart viewer to visualize the machine schedules.

![Gantt Preview](<images/image copy 3.png>)
*Interactive Gantt chart preview showing job distribution across machines over time.*

![Gantt Export](<images/image copy 8.png>)
*Logs confirming the successful export of the Gantt chart as an image.*

---

### 4. Data Management & Persistence
Solutions can be exported for later use or imported to verify results.

![Exporting Solution](<images/image copy 4.png>)
*Exporting the calculated solution to a `.jssp` compatible format.*

![Loading Solution](<images/image copy 5.png>)
*Using the file explorer to load a previously saved solution.*

![Solution Loaded](<images/image copy 6.png>)
*Logs confirming that the external solution was successfully parsed and loaded.*

![Direct Preview](<images/image copy 7.png>)
*Directly visualizing a loaded solution without re-running the solver.*

---

## 🚀 Key Features
- **Multiple Algorithms:** Support for different JSSP solving strategies.
- **Real-time Visualization:** Built-in Gantt chart rendering.
- **Import/Export:** Save solutions to disk and reload them for analysis.
- **Detailed Logging:** Comprehensive feedback on solver performance and file I/O.