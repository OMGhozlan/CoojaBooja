#!/usr/bin/env python
import numpy.random as random
from numpy import ones
from osmnx.distance import great_circle_vec
from geopy.distance import geodesic as VincentyDistance

def movements_generator(task_list):
    """[summary]

    Args:
        task_list ([type]): [description]

    Returns:
        [type]: [description]
    """
    movements = list()

    for task in task_list:
        task_id, task_day, task_hour, task_minute, task_dur, task_value = task
        move_count = int(task_dur/8)
        
        for i in range(move_count):
            iter_offset = i * 8
            remaining_time = task_dur
            day = task_day
            hour = task_hour
            minute = task_minute
            x, y = random.normal(), random.normal()
            r_seed = random.binomial(33, 0.5)
            f1 = random.choice(random.dirichlet(ones(r_seed)), p=random.dirichlet(ones((r_seed))))
            f2 = r_seed/100
            distance = f1 if r_seed%2 == 0 else f2
            origin = (y, x)
            bearing = 2 * sum(random.random(133))
            destination = VincentyDistance(kilometers=distance).destination(origin, bearing)
            y, x = destination.latitude, destination.longitude
            minute += iter_offset
            if(minute > 59):
                hour += 1
                minute %= 60
                if(hour > 23):
                    day += 1
                    hour %= 24
            remaining_time -= iter_offset
            movements.append([i + 1, float(y), float(x), day, hour, minute, task_dur, remaining_time])

    movements.sort(key= lambda movements: movements[3])
    return movements


def movements_2_txt(movements):
    """Converts movement data into a text file

    Args:
        movements ([list]): A list containing all data regarding the user movement
    Returns:
        int : status code
    """
    txt_file = open('movements.txt', 'w')
    txt_file.write("/Task_ID/ /X/ /Y/ /Day/ /Hour/ /Minute/ /Duration/ /Remaining Time/\n")
    for movement in movements:
        for item in movement:
            txt_file.write(f'{item}\t')
        txt_file.write('\n')
    txt_file.close()
    return 1


if __name__ == '__main__':
    from taskgen import task_generator, num_tasks, days
    print('[*]Generating tasks....')
    tasks = task_generator(num_tasks, days)
    print('[*]Generating user movement....')
    movements = movements_generator(task_list=tasks)
    print('[*]Writing user movement to file....')
    movements_2_txt(movements)
    print('[*]Done!')    