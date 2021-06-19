import numpy.random as random

def task_generator(num_tasks, days):
    """[summary]

    Args:
        num_tasks ([type]): [description]
        days ([type]): [description]

    Returns:
        [type]: [description]
    """
    tasks = list()
    for i in range(num_tasks):
        task_day = random.choice(range(1, days + 1))

        #Task hour and minute
        distrib = random.randint(1, 10)
        if distrib <= 4:
            task_hour = random.randint(9, 10)
        elif 4 < distrib <= 7:
            task_hour = random.randint(12, 16)
        else:
            task_hour = random.randint(18, 19)
        task_minute = random.randint(0, 59)

        # Task duration
        distrib = random.randint(1, 10)
        if distrib <= 5:
            task_dur = random.choice([20, 40, 60])
        elif 5 < distrib <= 9:
            task_dur = random.choice([30, 50, 70])
        else:
            task_dur = random.choice([10, 80, 100])
        
        task_value = round(random.uniform(1, 10))
        tasks.append([i+1, task_day, task_hour, task_minute, task_dur, task_value])
    return tasks


def tasks_2_txt(tasks):
    """[summary]

    Args:
        tasks ([type]): [description]

    Returns:
        [type]: [description]
    """
    txt_file = open('Tasks.txt', 'w')
    txt_file.write("/Task_ID/ /Day/ /Hour/ /Minute/ /Duration/ /Task_Value/\n")
    for task in tasks:
        for item in task:
            txt_file.write(f'{item}\t')
        txt_file.write('\n')
    txt_file.close()
    return 1

def get_params():
    """[summary]

    Returns:
        [type]: [description]
    """
    num_usr, days = None, None
    with open('Setup.txt', 'r') as data:
        count=0
        for line in data:
            if count==1:
                l = line.split()
                days = int(l[4])
            if count==4:
                l = line.split()
                num_usr = int(l[4])
                break
            count+=1
    return num_usr, days

if __name__ == '__main__':
    num_tasks, days = 1000, 2
    print('[*]Generating tasks....')
    tasks = task_generator(num_tasks, days)
    print('[*]Writing tasks to file....')
    tasks_2_txt(tasks)
    print('[*]Done!')