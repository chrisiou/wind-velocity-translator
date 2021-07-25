import sys
import random
from datetime import datetime, timedelta

TIME_FORMAT = '%Y-%m-%d %H:%M:%S'

''' 
here you can change the time period of wind's velocities generation and the start time's wind velocity
'''
CURRENT_TIME = datetime.strptime('2021-05-23 00:30:00', TIME_FORMAT) # start time
END_TIME = datetime.strptime('2021-05-23 23:30:30', TIME_FORMAT)
CURRENT_WIND_VELOCITY = 47999 # must be greater than 0

random.seed(CURRENT_WIND_VELOCITY)

class WindVelocity(object):
    def __init__(self, velocity):
        self.velocity = velocity

    def __eq__(self, other):
        return self.velocity == other.velocity

    def __next__(self):
        self.velocity += random.randrange(-1000, 1000)
        if (self.velocity < 0):
            self.velocity *= -1
        return self

class WindTimestamp(object):
    def __init__(self, current_time=CURRENT_TIME, current_vel=CURRENT_WIND_VELOCITY):
        self.current_time = current_time
        self.current_velocity = WindVelocity(current_vel)
        self.__prev_velocity = WindVelocity(None) # private member

    def __iter__(self): # made WindTimestamp an iterable object
        return self
        
    def __next__(self): # generates next timestamp and velocity
        self.current_time += timedelta(seconds = random.randint(1, 60)) # moves time to the future random number seconds 
        self.__prev_velocity.velocity = self.current_velocity.velocity
        
        next(self.current_velocity) # generates another wind velocity for this current time
        if self.current_velocity == self.__prev_velocity: # if the new generated velocity is the same with the last one, calculates a new time, velocity pair
            return next(self)

        timestamp = str(self.current_time.strftime(TIME_FORMAT))
        return '{0},{1}\n'.format(timestamp, self.current_velocity.velocity) # returns the "timestamp, velocity" as string to be pushed in the output file

def main():
    try:
        outfile = sys.argv[1]
    except IndexError:
        print('Give output file')
        sys.exit(1)
    
    # initialize
    timestamp = WindTimestamp(CURRENT_TIME, CURRENT_WIND_VELOCITY)
    wind = iter(timestamp)

    with open(outfile, 'w') as f:
        # f.write("date, wind velocity\n")
        while (timestamp.current_time <= END_TIME):
            f.write(next(wind))

if __name__ == '__main__':
    main()
