********************************************************************IMPORTANT********************************************************************

--------------------------------------------------------------------SETUP*FILE-------------------------------------------------------------------

- "Setup.txt file" contains the input information useful for the simulation:

	- Days of simulation: it is possible to select from 0 to 6. It is possible to simulate maximum 7 days.  
	- Number of users: total number of pedestrians moving inside the city. 
	- Minimum travel time: minimum travel time for each user.
	- Maximum travel time: maximum travel time for each user.
	- Start hour simulation, Start minute simulation: initial time of the simulation.	
	- Finish hour simulation, Finish minute simulation: finish time of the simulation.
	- Kind of antennas: antennas system used.


	- Create a new list of events: it is possible choose to create a new list of events or usigng the default one.
          if it is decided to create a new list of events, it is important to choose:
	
	- Integer value for the ray(meters): essential vale for the simulation. It is important to create the list of adjacent points contained
	  inside the "MapGraphPoints.txt" file. At each movement time, a generic user will choose the next point according the list of adjacent 	  points.

-------------------------------------------------------------------------------------------------------------------------------------------------

- "CoordinatesAntennas.txt" file contains the list of the antennas position useful for the simulation. 
	
	- ID-Antenna: antenna identifier.	
	- Lat, Long: antenna location.

- "MapPointsCenter.txt" file contains the list of the possible users position inside the considered city, in this case Luxembourg.

	- Lat, Long, Alt: point location inside the city.


