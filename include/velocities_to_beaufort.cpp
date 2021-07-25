#include "./velocities_to_beaufort.h"

void MinuteVelocity::add_to_seconds_velocities_set(double s) { seconds_velocities.insert(s);}

void MinuteVelocity::clear_seconds_velocities_set(void) { seconds_velocities.clear();}

double MinuteVelocity::average_velocity(void) {
    double total = 0.0;
    for(auto iter = seconds_velocities.cbegin(); iter != seconds_velocities.cend(); ++iter) { total += *iter;}
    return total/seconds_velocities.size();;
}

MovingMean::MovingMean(int N) {
    if (N < 1) {
        std::cout << "Size must be greater than 0" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    queue.reserve(N);
    size = N;
    num_of_elements = 0;
    front = 0;
    sum_MM = 0.0;
    MM = 0.0;
}

bool MovingMean::is_queue_full(void) {
    if (num_of_elements == 0) return false;
    return ((front+num_of_elements)%size == front);
}

bool MovingMean::is_queue_empty(void) { return (num_of_elements == 0);}

bool MovingMean::push(double value) {
    if (this->is_queue_full()) {
        std::cout << "Queue is full! Insertion of element: " << value << " is cancelled!" << std::endl;
        return false;
    }
    queue[(front+num_of_elements)%size] = value; 
    if (num_of_elements < size) {num_of_elements++;}
    sum_MM += value;
    return true;
}

double MovingMean::pop(void) {
    if (this->is_queue_empty()) {
        std::cout << "Queue is empty! There is no element to pop!" << std::endl;
        exit(EXIT_FAILURE);
    }
    double value = queue[front];
    front = (front+1)%size;
    num_of_elements--;
    return value;
}

double MovingMean::get_MM(void) const { return MM;}

bool less_than_an_hour(std::string start_time, std::string end_time) {
    int hour = std::stoi(end_time.substr(11,2));
    hour -= std::stoi(start_time.substr(11,2));
    if (start_time[12] != end_time[12]) { hour += 60;}
    return (hour < 60);
}

void MovingMean::calculate_MM(bool calc_MM) {
    if (size < 1) return;
    if (calc_MM || (num_of_elements == size)) { 
        MM = sum_MM / num_of_elements;
        sum_MM -= this->pop();
    }
}

void HourBeaufort::set_status(void) {
    if (last_velocities.get_MM() <= 0) {
        code = -1;
        max_wave_height = -1;
        sea_state = "Not enough data to calculate the average velocity or sth goes wrong with the anemometer";
    } else if (last_velocities.get_MM() < 1000) {
        code = 0;
        max_wave_height = 0;
        sea_state = "calm / mirror";
    } else if (last_velocities.get_MM() <= 5000) {
        code = 1;
        max_wave_height = 0.1;
        sea_state = "calm / ripples";
    } else if (last_velocities.get_MM() <= 11000) {
        code = 2;
        max_wave_height = 0.5;
        sea_state = "smooth / sm wavelets";
    } else if (last_velocities.get_MM() <= 19000) {
        code = 3;
        max_wave_height = 1.25;
        sea_state = "slight / lg wavelets";

    } else if (last_velocities.get_MM() <= 28000) {
        code = 4;
        max_wave_height = 2.5;
        sea_state = "moderate / sm breaking waves";

    } else if (last_velocities.get_MM() <= 38000) {
        code = 5;
        max_wave_height = 4;
        sea_state = "rough / moderate waves & foam";

    } else if (last_velocities.get_MM() <= 49000) {
        code = 6;
        max_wave_height = 6;
        sea_state = "lg waves - foam - spray";
        
    } else if (last_velocities.get_MM() <= 61000) {    
        code = 7;
        max_wave_height = 6;
        sea_state = "very rough / sea heads up streaked foam";

    } else if (last_velocities.get_MM() <= 74000) {
        code = 8;
        max_wave_height = 9;
        sea_state = "higher long waves foam in streaks";

    } else if (last_velocities.get_MM() <= 88000) {
        code = 9;
        max_wave_height = 9;
        sea_state = "high / high waves - dense foam -impaired visibility ";

    } else if (last_velocities.get_MM() <= 102000) {
        code = 10;
        max_wave_height = 14;
        sea_state = "very high / very high tumbling waves - surface white with foam - visibility affected ";

    } else {
        code = 11;
        max_wave_height = 100;
        sea_state = "phenomenal / exceptionally high waves - sea covered in foam - visibility affected ";
    }
}

void HourBeaufort::add_velocity(double velocity, std::string current_time) {
    last_velocities.push(velocity);

    bool to_calc_MM = false;
    if (not less_than_an_hour(time, current_time)) to_calc_MM = true;
    last_velocities.calculate_MM(to_calc_MM);
    this->set_status();
} 

double HourBeaufort::get_last_hour_average_wind_vel(void) const { return last_velocities.get_MM();}

int HourBeaufort::get_code(void) { return code;}

std::string HourBeaufort::get_time(void) { return time;}

void HourBeaufort::set_time(std::string _time) { time = _time;}

std::ostream& operator<<(std::ostream& os, const HourBeaufort& timestamp) {
    double vel = timestamp.get_last_hour_average_wind_vel();
    os << timestamp.time.substr(0,16) << "," << timestamp.code <<','<< vel << ',' << timestamp.max_wave_height << ',' << timestamp.sea_state << '\n';
    return os;
}
