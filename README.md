
# UNIX Programming and calls

This project implements a series of exercises designed to practice UNIX programming and system calls. 
It includes process management, thread creation, and file manipulation.

## Features

### Functions

1. **`pid_t my_fork(void)`**:
   - Creates a new process using `fork()`.
   - Returns the PID of the new process on success.
   - Prints an error message and exits the program if `fork()` fails.

2. **`void print_pids(int fd, short unsigned int N, short unsigned int G)`**:
   - Generates processes up to `G` generations, each printing its PID and generation.
   - The output is written to the specified file.

3. **`void count_lines(short unsigned int G)`**:
   - Counts the number of lines in `out.txt` for each generation using `grep` and `wc`.
   - Prints the line counts in decreasing order of generations.

4. **`void print_threads(short unsigned int N)`**:
   - Creates `N` threads, each printing "Hi. I'm thread number i" in the correct order.

5. **`int main(int argc, char* argv[])`**:
   - Accepts two arguments:
     - `N`: Number of iterations per process/thread.
     - `G`: Number of generations for process creation.
   - Executes the tasks:
     1. Writes process details to `out.txt`.
     2. Counts and prints the lines for each generation.
     3. Creates threads and prints their messages.

### Example

Running:
```bash
make
./OS 3 2
```

Produces:
1. A file `out.txt` containing:
   ```
   My pid is 12345. My generation is 0.
   My pid is 12346. My generation is 1.
   ...
   ```

2. Output to the screen:
   ```
   Number of lines by processes of generation 2 is 9
   Number of lines by processes of generation 1 is 3
   Number of lines by processes of generation 0 is 1
   Hi. I'm thread 0
   Hi. I'm thread 1
   Hi. I'm thread 2
   ```

## How to Build and Run

1. Build the project:
   ```bash
   make
   ```

2. Run the executable:
   ```bash
   ./OS <N> <G>
   ```

   - `N`: Number of iterations per process/thread.
   - `G`: Number of generations for process creation.

## Dependencies

- UNIX-compatible operating system.
- GCC compiler.

## File Structure

- `main.c`: Contains the implementation of all functions.
- `Makefile`: Automates the compilation process.
