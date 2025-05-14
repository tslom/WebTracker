<h3 align="center">WebTracker</h3>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#resources">Resources</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

WebTracker is a lightweight, terminal-based network monitoring tool built in C++. It captures and analyzes live network packets, logs real-time statistics, and provides options to visualize tracked data over time.

This project is ideal for:
- Students learning about packet capture
- Developers interested in traffic profiling
- Lightweight diagnostics for network behavior


<!-- GETTING STARTED -->
## Getting Started

To get a local copy of the code up and running, follow these steps.

### Prerequisites

First, you need to install Gnuplot and Boost which are prerequisites for other packages in my project.
* Homebrew
  ```sh
    brew install gnuplot boost
  ```

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/tslom/WebTracker.git
   ```
2. Run Cmake to install other packages (matplot and pcapplusplus) and build the code
   ```sh
    mkdir -p build
    cd build
    cmake .. -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    make
    cd ..
   ```


<!-- USAGE EXAMPLES -->
## Usage

1. Start the program
   ```sh
    sudo ./WebTracker
   ```
   
2. Start collecting the packet data
   ```sh
    start
   ```

3. Parse the packet data
   ```sh
    updatedata
   ```

4. If you want to stop collecting data
   ```sh
    stop
   ```

5. Graph data with respect to time
   ```sh
   # to check what categories can be checked
   graphtime
   
   # generate a graph
   graphtime [category]
   ```

6. Exit the project
   ```sh
   exit
   ```


<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<!-- RESOURCES -->
## Resources

* [README template](http://github.com/othneildrew/Best-README-Template/blob/main/BLANK_README.md)
* [PcapPlusPlus Tutorials](https://pcapplusplus.github.io/docs/tutorials/)
* [Cmake Documentation](https://cmake.org/cmake/help/v2.8.8/cmake.html)
* [Matplot++](https://alandefreitas.github.io/matplotplusplus/)
