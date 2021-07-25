#ifndef VELOCITIES_TO_BEAUFORT_H
#define VELOCITIES_TO_BEAUFORT_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

class MovingMean {
    std::vector<double> queue; // a fixed size container which holds the wind velocity values of the last hour to calculate the moving average
    int front;
    int size;
    int num_of_elements;
    double sum_MM = 0.0;

    double MM = -1; // moving average of an hour's velocities

public:
    MovingMean(int N);

    bool is_queue_full(void);
    bool is_queue_empty(void);
    bool push(double value);
    double pop(void); // pop() doesn't delete any item, just let it be overwritten by push

    double get_MM(void) const;
    void calculate_MM(bool calc_MM=false); // when calc__MM = true, the function calculates the MM independently of total time.
};

class MinuteVelocity  {
    std::unordered_set<double> seconds_velocities; // keep the velocities values in order to calculate the average velocity of this minute

public:
    void add_to_seconds_velocities_set(double s);
    void clear_seconds_velocities_set(void);
    double average_velocity(void);
};

class HourBeaufort {
    // wind state information
    int code = -1;
    int max_wave_height = -1;
    std::string time = "";
    std::string term = "";
    std::string sea_state = "";

    MovingMean last_velocities{60}; // it can possibly have maximum 60 velocities input per hour

protected:
    void set_status(void);

public:
    HourBeaufort(std::string _time) : time(_time) {};

    void set_time(std::string _time);
    std::string get_time(void);
    int get_code(void);
    double get_last_hour_average_wind_vel(void) const; // returns moving mean average velocity
    void add_velocity(double velocity, std::string current_time);

    friend std::ostream& operator<<(std::ostream& os, const HourBeaufort& time); 
};

bool less_than_an_hour(std::string start_time, std::string end_time);

#endif