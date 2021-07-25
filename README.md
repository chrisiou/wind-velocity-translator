# Wind velocity to Beaufort conversion algorithms

## Table of Contents

- [Description](#description)
- [Velocities conversion to Beaufort per hour](#toBeauforts)
    - [Input](#input)
    - [Moving mean](#movingmean)
    - [Structures](#structures)
- [Complexities](#complexities)
- [Compile & Run](#compilerun)
    - [Makefile](#makefile)
    - [Command line](#commandline)

## Description <a id="description"></a>

A toy project which translates wind velocities to Beaufort codes.

A Python program generates wind velocities values in a specific time period and exports them to an output file.

A C++ program parses an input file with *timestamp, velocity* pairs, finds the average wind velocity for each minute and keeps these minutes' speeds in a fixed size queue in order to calculate a moving mean (MM) wind velocity of the last hour. This MM wind velocity determines the hour's Beaufort code and export this code (with some extra information) to a csv output file (named **beaufort.csv**).

## Velocities conversion to Beaufort per hour <a id="toBeauforts"></a>

file: translator.cpp

This program determines Beaufort wind codes based on the wind's velocities values of the **last** hour. The input data usually come with a few seconds long time intervals, so firstly it finds the minute's average speed and then holds this value in a container. When the timestamps in the container complete an hour period the program calculates the moving average wind velocity.

### Input <a id="input"></a>

It takes as input a file with *timestamp, velocity* pairs in format: **yyyy-mm-dd hh:mm:ss,velocity**

## Moving mean <a id="movingmean"></a>

In statistics, a moving average (moving mean, rolling average or running average) is a calculation to analyze data points by creating a series of averages of different subsets of the full data set. It is the calculation of the mean over the **last** k data-points.

source: https://en.wikipedia.org/wiki/Moving_average

### Structures <a id="structures"></a>

`MovingMean` calculates a moving average of a series of `double` values

To calculate the moving mean of velocities, we need a container to hold the velocities of an hourly period and after the calculation of MM to remove the first velocity entered to this container. In order to get ***space complexity: O(1)***, a std::vector is used as a fixed size queue.

- This queue of length `N` can be defined by the constructor `MovingMean(int N)`. No need for implicitly-defined destructor, all members have trivial destructors.
- `push()` adds a new element at the last valid position by wrapping around to the size of the queue. This means that it adds at `(front + num_of_elements_in_the_Q)mod(size_of_Q)` position.
- `pop()` returns the value which is pointed by the front member. To keep constant size for the queue, pop() does **not** delete any item, it lets it to be overwritten by `push()`. 
- `is_queue_full()` checks if there is any valid space to be overwritten by `push()`.
- `calculate_MA()` calculates a moving average velocity per hour. There is an optional argument `bool calc_MM` (defaulted as `false`). When is assigned as `true`, calculates the MM independently of the passed time.

`MinuteVelocity` finds the minute's average speed

- An `std::unordered_set` named `seconds_velocities` keeps the velocities which belong to this minute.
- `average_velocity()` calculates the average velocity of this minute by taking the values of `seconds_velocities` set.

`HourBeaufort` collects and exports the hour's wind information

- It has a `MovingMean` queue container of fixed size `60`.
- The `add_velocity(double velocity, std::string current_time)` method pushes `velocity` to the `MovingMean` queue. If the `current_time` is after an hour of the `HourBeaufort`'s hour, it calculates the MM for this hour.

## Complexities <a id="complexities"></a>

***Time Complexity: O(N)***  
***Space Complexity: O(1)***  
where

- `N` stands for the number of *timestamp, wind velocity* pairs in the input file.
The program is processing line by line without using an internal buffer. Furthermore, the operations of the fixed size queue and of the unordered_set have **O(1)** time complexities.
- fixed size queue in the `MovingMean` class has constant space complexity.
- unordered_set will probably hold maximum number of 60 values, so it has also constant space complexity.

## Compile & run <a id="compilerun"></a>

To generate the market data

```python
python3 wind_timestamps_generator.py <PATH_TO_OUTPUTFILE>
```

*e.g. `<PATH_TO_OUTPUTFILE`: timestamps.csv*

### Command line <a id="commandline"></a>

To compile C++ source files without Makefile

```bash
g++ translator.cpp include/velocities_to_beaufort.cpp -Wall -Wextra -o beaufort.x
```

To run the program

```bash
./beaufort.x <PATH_TO_INPUTFILE>
```

### Makefile <a id="makefile"></a>

To compile the C++ source files

```bash
make
```

To run the program

```bash
make run arg=<PATH_TO_INPUTFILE>
```

To remove the ELF

```bash
make clean
```

*note: `PATH_TO_INPUTFILE` = `PATH_TO_OUTPUTFILE`*