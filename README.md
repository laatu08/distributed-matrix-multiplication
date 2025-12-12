# Matrix Multiplication Performance Comparison

A comprehensive project demonstrating different approaches to matrix multiplication, including sequential, multithreaded, and distributed computing implementations.

## Project Overview

This project implements and compares three different approaches to matrix multiplication:

1. **Test Data Generator** - C++ program to generate random matrix input
2. **Multithreaded Implementation** - C++ programs using std::thread for parallel computation
3. **Distributed Implementation** - Rust-based client-server architecture for distributed computing

## Directory Structure

```
.
├── generator.c++                              # Test data generator
├── multithreaded/
│   ├── matrix_thread.cpp                      # Basic multithreaded multiplication
│   ├── matrix_thread_timing.cpp               # Performance comparison tool
│   └── input.txt                              # Generated test input
└── worker/
    └── distributed-matrix-multiplier/
        ├── client/                            # Rust client application
        │   ├── Cargo.toml
        │   └── src/main.rs
        └── server/                            # Rust server application
            ├── Cargo.toml
            └── src/main.rs
```

## Components

### 1. Test Data Generator (`generator.c++`)

Generates random matrices for testing matrix multiplication implementations.

**Features:**
- Creates square matrices of configurable size (default: 300x300)
- Generates random integer values (0-9)
- Outputs to `input.txt` in the format: `N M P` followed by matrix A and matrix B

**Build & Run:**
```bash
g++ generator.c++ -o generator
./generator
```

**Configuration:**
- Modify the `N` variable in the code to change matrix size (e.g., 300, 500, 800)

### 2. Multithreaded Implementation

#### Basic Multithreaded (`multithreaded/matrix_thread.cpp`)

Implements parallel matrix multiplication using C++ threads.

**Features:**
- Automatically detects number of available CPU cores
- Distributes rows evenly across threads
- Shows thread ID and row assignments for debugging
- Handles workload balancing for uneven divisions

**Build & Run:**
```bash
cd multithreaded
g++ matrix_thread.cpp -o matrix_thread -std=c++11 -pthread
./matrix_thread < input.txt
```

**Manual Input:**
```bash
./matrix_thread
# Enter: N M P
# Enter: Matrix A values
# Enter: Matrix B values
```

#### Performance Timing (`multithreaded/matrix_thread_timing.cpp`)

Compares single-threaded vs multi-threaded performance.

**Features:**
- Executes both single-threaded and multi-threaded multiplication
- Measures execution time for both approaches
- Calculates speedup factor
- Uses high-resolution clock for accurate timing

**Build & Run:**
```bash
cd multithreaded
g++ matrix_thread_timing.cpp -o matrix_thread_timing -std=c++11 -pthread
./matrix_thread_timing < input.txt
```

**Output Example:**
```
Performance Results
-------------------
Matrix size: 300 x 300 * 300 x 300
Single-thread time : 2.45 seconds
Multi-thread time  : 0.68 seconds
Speedup            : 3.60x
```

### 3. Distributed Implementation (Rust)

A client-server architecture that distributes matrix multiplication across multiple worker nodes.

#### Server (`worker/distributed-matrix-multiplier/server`)

**Features:**
- Listens on port 8080 for client connections
- Splits matrix A into chunks for distribution
- Sends matrix chunks and full matrix B to clients
- Collects and combines results from all clients
- Currently configured for 2 clients with 4x4 matrices

**Build & Run:**
```bash
cd worker/distributed-matrix-multiplier/server
cargo build --release
cargo run
```

**Expected Flow:**
1. Server starts and listens on port 8080
2. Waits for 2 client connections
3. Distributes first half of matrix A to client 1
4. Distributes second half of matrix A to client 2
5. Collects partial results
6. Combines and displays final result

#### Client (`worker/distributed-matrix-multiplier/client`)

**Features:**
- Connects to server on port 8080
- Receives assigned matrix rows and full matrix B
- Performs matrix multiplication with detailed tracing
- Shows step-by-step calculation process
- Sends computed results back to server

**Build & Run (in separate terminals):**

Terminal 1 (Server):
```bash
cd worker/distributed-matrix-multiplier/server
cargo run
```

Terminal 2 (Client 1):
```bash
cd worker/distributed-matrix-multiplier/client
cargo run
```

Terminal 3 (Client 2):
```bash
cd worker/distributed-matrix-multiplier/client
cargo run
```

**Output:**
- Shows received payload from server
- Displays detailed multiplication steps
- Prints computed partial result
- Confirms result transmission to server

## Requirements

### C++ Components
- **Compiler**: g++ with C++11 support
- **Libraries**: pthread (for threading support)
- **OS**: Linux, macOS, or Windows with MinGW

### Rust Components
- **Rust**: 1.70.0 or higher
- **Cargo**: Latest stable version
- **Dependencies** (automatically managed):
  - serde 1.0 (serialization)
  - serde_json 1.0 (JSON handling)
  - rand 0.9.2 (server only - random number generation)

## Usage Workflows

### Testing Multithreaded Performance

1. Generate test data:
```bash
./generator
```

2. Run timing comparison:
```bash
cd multithreaded
./matrix_thread_timing < input.txt
```

3. Analyze the speedup factor and compare execution times

### Running Distributed Multiplication

1. Start the server:
```bash
cd worker/distributed-matrix-multiplier/server
cargo run
```

2. In separate terminals, start two clients:
```bash
cd worker/distributed-matrix-multiplier/client
cargo run
```

3. Observe the distributed computation process and final result

## Implementation Details

### Multithreaded Approach

**Algorithm:**
- Uses row-based parallelization
- Each thread computes a subset of rows in the result matrix
- Workload is balanced using `rows_per_thread + extra` distribution
- Thread count limited to available CPU cores or matrix rows (whichever is smaller)

**Performance Characteristics:**
- Best for medium to large matrices (300x300+)
- Speedup depends on CPU core count
- Memory shared across threads (efficient)

### Distributed Approach

**Algorithm:**
- Server splits matrix A horizontally
- Each client receives:
  - Assigned rows of matrix A
  - Complete matrix B
- Clients compute partial results independently
- Server aggregates results in order

**Communication Protocol:**
- JSON-based serialization using serde
- Newline-delimited messages
- Synchronous request-response pattern

**Performance Characteristics:**
- Suitable for very large matrices
- Network latency overhead
- Horizontal scalability
- Currently hardcoded for 2 clients (extensible)

## Performance Considerations

### Multithreaded
- **Pros**: Low overhead, shared memory, automatic core detection
- **Cons**: Limited by single machine's CPU cores
- **Best for**: Medium-sized matrices (100-1000)

### Distributed
- **Pros**: Unlimited horizontal scaling, fault isolation
- **Cons**: Network overhead, serialization cost, coordination complexity
- **Best for**: Very large matrices or when multiple machines are available

## Customization

### Changing Matrix Size (Generator)
Edit `generator.c++`:
```cpp
int N = 500; // Change to desired size
```

### Changing Number of Clients (Distributed)
Edit `server/src/main.rs`:
- Modify the split logic in `multiply_distributed()`
- Adjust the loop that accepts client connections
- Update result aggregation logic

### Changing Server Port
Edit both `server/src/main.rs` and `client/src/main.rs`:
```rust
const PORT: i32 = 9090; // Change to desired port
```
- [Rust Serde](https://serde.rs/)
- [Parallel Computing Algorithms](https://en.wikipedia.org/wiki/Parallel_algorithm)
