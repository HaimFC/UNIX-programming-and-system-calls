
# UNIX Programming and System Calls

This project consists of several exercises designed to practice UNIX programming concepts and system calls. The program includes features such as process and thread creation, PID management, and inter-process communication.

## Features

### Functions

1. **`pid_t my_fork(void)`**:
   - Calls `fork()` and returns the value on success.
   - Prints an error message and exits if `fork()` fails.

2. **`void print_pids(int fd, short unsigned int N, short unsigned int G)`**:
   - Creates processes up to generation `G`, each process printing its PID and generation.
   - Outputs the result to a specified file descriptor.

3. **`void count_lines(short unsigned int G)`**:
   - Counts the number of lines in a file (`out.txt`) and prints statistics about generations.

4. **`void print_threads(short unsigned int N)`**:
   - Creates `N` threads, each printing a message in the correct order.

5. **`int main(int argc, char* argv[])`**:
   - Opens/creates the file `out.txt`.
   - Calls `print_pids`, `count_lines`, and `print_threads`.

### Output

- The program outputs lines to `out.txt` in a structured format.
- Prints line count statistics for each generation to the screen.
- Displays thread messages in the correct order.

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
   Hi. I'm thread number 0
   Hi. I'm thread number 1
   Hi. I'm thread number 2
   ```

## How to Build and Run

1. Compile the code:
   ```bash
   make
   ```

2. Run the executable:
   ```bash
   ./OS <N> <G>
   ```

   - `N`: Number of iterations per process.
   - `G`: Number of generations.

## Dependencies

- UNIX-compatible operating system.
- GCC compiler.

## File Structure

- `main.c`: Contains the implementation of all functions.
- `Makefile`: Compiles the code into an executable named `OS`.

