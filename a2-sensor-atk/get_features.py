from NetworkFlowMeter.TicToc import timing, Timer
from NetworkFlowMeter import *

i = 1

@timing
def main():
    global i
    pcapPath = f'output_pcap/a2_{i}.pcap'
    csvFile = f'output_pcap/a2_{i}.csv'
    pcap2csv(pcapPath)
    i += 1
    pass


if __name__ == '__main__':
    for _ in range(3):
        main(timerPrefix='Total Time Costs: ', timerBeep=False)
