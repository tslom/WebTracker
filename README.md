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

Here's a blank template to get started. To avoid retyping too much info, do a search and replace with your text editor for the following: `github_username`, `repo_name`, `twitter_handle`, `linkedin_username`, `email_client`, `email`, `project_title`, `project_description`, `project_license`



<!-- GETTING STARTED -->
## Getting Started

To get a local copy of the code up and running follow these simple steps.

### Prerequisites

First, you need to install Gnuplot and Boost which are prerequisites for other packages in my project.
* Homebrew
  ```sh
    brew install gnuplot boost
  ```

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/github_username/repo_name.git
   ```
2. Run Cmake to install other packages (matplot and pcapplusplus) and build the code
   ```sh
    mkdir -p build
    cd build
    cmake ..
    make
   ```


<!-- USAGE EXAMPLES -->
## Usage

1. First, start collecting the packet data
   ```sh
    ./WebTracker start
   ```
   
2. Parse the packet data
   ```sh
    ./WebTracker start
   ```

3. If you want to stop collecting data
   ```sh
    ./WebTracker stop
   ```

3. Graph data with respect to time
   ```sh
   # to check what categories can be checked
   ./WebTracker graphtime
   
   # generate a graph
   ./WebTracker graphtime [category]
   ```


<!-- LICENSE -->
## License

Distributed under the project_license. See `LICENSE.txt` for more information.



<!-- RESOURCES -->
## Resources

* [README template](http://github.com/othneildrew/Best-README-Template/blob/main/BLANK_README.md)
* [PcapPlusPlus Tutorials](https://pcapplusplus.github.io/docs/tutorials/)
* [Cmake Documentation](https://cmake.org/cmake/help/v2.8.8/cmake.html)
* []()
