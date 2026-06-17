# NetLatencyInjector ![Alpha](https://img.shields.io/badge/Status-Release-green)

**Lagger** is a lightweight Linux utility for injecting artificial network latency using `tc netem`. Designed for developers, testers, or anyone curious about how their apps behave under lag, it provides an intuitive terminal interface for applying and monitoring delay on any network interface.

## Features

- Add custom network delay in milliseconds per interface
- View current delay status of all interfaces at a glance
- Vim-style keyboard navigation
- Simple TUI built in Rust with [ratatui](https://github.com/ratatui-org/ratatui)

## Prerequisites

- Linux system with `tc` (from the `iproute2` package)
- `sudo` privileges
- [Rust toolchain](https://rustup.rs) (stable)

## Installation

```bash
# Clone the repository
git clone https://github.com/ItalianG0urmet/NetLatencyInjector.git
cd NetLatencyInjector

# Build the project
cargo build --release

# Run the tool (requires sudo for tc commands)
sudo ./target/release/lagger
sudo ./target/release/NetLatencyInjector
```

## Usage

| Key       | Action                              |
|-----------|-------------------------------------|
| `j` / `k` | Move selection down / up            |
| `Enter`   | Open delay input for selected interface |
| `Esc`     | Close input without applying        |
| `q`       | Quit                                |

Select an interface, press `Enter`, type the desired delay in milliseconds, and confirm with `Enter` again. The current delay for each interface is shown inline next to its name.

## Example commands used internally

Lagger internally wraps standard `tc` commands like:

```bash
tc qdisc replace dev <iface> root netem delay 100ms
tc qdisc show dev <iface>
```
