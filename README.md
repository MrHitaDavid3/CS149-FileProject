## Overview
A simplified file-management system implemented in C on Linux. The program simulates file storage in memory and supports creating, searching, opening, editing, and closing files.

Each operation runs in a separate worker thread, with synchronization handled through a mutex, condition variable, and shared command state to prevent race conditions and coordinate execution.

**Final Report**: [Link to Report](https://docs.google.com/document/d/1Fojyz6wmcmaDN5t-Mf9yXc5Uko3MD2X-cEQqpiIpz3k/edit?usp=sharing)

## Setup
REQUIREMENT:
sudo apt install libreadline-dev

How to compile:
make

How to run:
./fileproject
