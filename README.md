# NetLatencyInjector ![Alpha](https://img.shields.io/badge/Status-Release-green)

**Lagger** is a lightweight Linux utility for injecting artificial network latency using `tc netem`. Designed for developers, testers, or anyone curious about how their apps behave under lag, it provides an intuitive interface for applying, checking, and removing delay on a selected network interface.

## Features
- Add custom network delay in milliseconds
- Remove existing delay rules
- Check current delay status
- Simple interface built in C++

## Prerequisites
- Linux system with `tc` (from the `iproute2` package)
- `sudo` privileges
- CMake & a C++ compiler (e.g., `g++`)

## Installation
```bash
# Clone the repository
git clone https://github.com/ItalianG0urmet/NetLatencyInjector.git
cd NetLatencyInjector

# Build the project
mkdir build
cd build
cmake ..
make

# Run the tool (requires sudo for tc commands)
sudo ./lagger
````

## 🛠 Example commands used internally

Lagger internally wraps standard `tc` commands like:

```bash
tc qdisc add dev <iface> root netem delay 100ms
tc qdisc show dev <iface>
tc qdisc del dev <iface> root netem
```

---
