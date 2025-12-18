# operating-systems-chat-project

A small collection of example chat programs implemented for operating-systems coursework.
This repository contains two independent implementations demonstrating inter-process
communication techniques in C++/Qt:

- "Sharing project" — a shared-memory + semaphore based chat (server + client) using POSIX shared memory and Qt for the GUI.
- "Socket Project" — a TCP socket based chat (server + client) implemented with Qt networking and GUI.

Both subprojects are Qt-based applications (qmake generated Makefiles). Use the instructions below to build and run each.

## Repository layout

Top-level folders of interest:

- `Sharing project/`
	- `client/` — shared-memory chat client (target: `ShmChatClient`)
	- `server/` — shared-memory chat server (target: `ShmChatServer`)

- `Socket Project/`
	- `SocketClientChat/` — socket chat client (target: `ChatClient`)
	- `SocketServerChat/` — socket chat server (target: `ChatServer`)

Each of the four subfolders contains a qmake project and a Makefile generated for Qt 5. Building is done inside the relevant folder.

## Prerequisites

- A modern C++ compiler (g++). The Makefiles were generated with g++ and use C++17/C++20 flags.
- Qt 5 development tools (qmake, moc) and Qt libraries. On Debian/Ubuntu this is typically provided by packages such as:

	- libqt5core5a, libqt5widgets5, libqt5network5
	- qt5-qmake, qtbase5-dev, qttools5-dev-tools

- make

Install example (Debian/Ubuntu):

```bash
sudo apt update
sudo apt install build-essential qtbase5-dev qt5-qmake qttools5-dev-tools libqt5network5-dev
```

Note: package names vary by distribution. If you use Qt6, you may need to regenerate Makefiles (`qmake`/`qmake6`) or open projects in Qt Creator.

## Build instructions

Build each component from its folder. Example commands (run from repository root):

```bash
# Shared-memory server
cd "Sharing project/server"
make

# Shared-memory client
cd "../client"
make

# Socket server
cd "../../Socket Project/SocketServerChat"
make

# Socket client
cd "../SocketClientChat"
make
```

If a Makefile is missing or you want to regenerate it, run `qmake` (or `qmake -qt=5`) in the subproject directory and then `make`.

## Run instructions

Run the server first, then start one or more clients to connect.

Shared-memory example (from `Sharing project`):

```bash
# from Sharing project/server after build
./ShmChatServer

# open a terminal per client
cd ../client
./ShmChatClient
```

Socket example (from `Socket Project`):

```bash
# from SocketServerChat after build
./ChatServer

# in another terminal (SocketClientChat)
./ChatClient
```

The GUI apps use Qt for the front-end. For simple troubleshooting you can run them from a terminal to see stdout/stderr messages.

## Notes & troubleshooting

- Qt version: Makefiles in this repo were generated with Qt 5 (qmake). If your environment uses Qt6, regenerate the project files with the appropriate qmake or open the `.pro` files with Qt Creator.
- Permissions: Shared memory usage typically does not require root, but if you see permission errors related to POSIX IPC or /dev/shm, review OS limits and permissions.
- If you get link errors about missing Qt libraries, ensure Qt dev packages are installed and that `qmake` from the expected Qt version is used.

## Development and testing

- The projects are small and GUI-based; unit tests are not provided. You can run a server and multiple clients locally to test messaging.
- To profile or instrument IPC behavior, add logging in `shm.h`/receiver files or run the applications under valgrind/address-sanitizer when debugging memory issues.

## License

See the `LICENSE` file at the repository root.

## Contact

If you need help building or running the projects, open an issue or reach out to the repository owner.

## Contributors

Thank you to everyone who contributed to this project:

- Ibraheem Mohammed — Leader
- Mahmoud Medhat
- Tarek Mostafa
- Dr. Waffa — "Doctor I will create project for you"

