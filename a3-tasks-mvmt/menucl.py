import sys, os
import shutil
# Main definition - constants
menu_actions  = {}  
# =======================
#     MENUS FUNCTIONS
# =======================
listsaved=[]
namecity='no'
usr=0
ndays=0

# Main menu
def main_menu1(saved,num_usr,days):
    os.system('clear')
    print ("Welcome, ** THIS IS THE CrowdSenSim SIMULATOR **\n")
    
    global listsaved
    listsaved=saved
    global usr
    usr=num_usr
    global ndays
    ndays=days
    
    return main_menu()

def main_menu():

    try:
        shutil.rmtree('./Inputs/Default/Mobility/chosen_list')
        os.makedirs('./Inputs/Default/Mobility/chosen_list')
    except:
        pass
    print ("Number of Users: "+str(usr))
    print ("Number of days: "+str(ndays))
    print ("\nPlease choose the menu you want to start:")
    print ("1. Use a saved list of events")
    print ("2. Create new list of events")
    print ("3. Manage saved list")
    print ("4. Change User or Days")
    print ("0. Quit")
    choice = input(" >>  ")
    exec_menu(choice)

    return listsaved,namecity,usr,ndays


# Execute menu
def exec_menu(choice):
    os.system('clear')
    ch = choice.lower()
    if ch == '':
        menu_actions['main_menu']()
    else:
        try:
            menu_actions[ch]()
        except KeyError:
            print ("Invalid selection, please try again.\n")
            menu_actions['main_menu']()
    return

# Menu 1
def menu1():
    print ('List of Events already saved :  ')
    for it in listsaved:
        print (it['id'],'.  city: ',it['name'],'  number of user: ',it['us'],' days ',it['days']  )  
    print ("b. Back")
    choice = input(" >>  ")

    exec_menu1(choice)
    return

def exec_menu1(choice):
    os.system('clear')
    global listsaved
    ch = choice.lower()
    if ch == '' or ch =='b':
        menu_actions['main_menu']()
    
    else:
        if ch.isdigit():
            if int(ch)<=listsaved[-1]['id']:
                if (os.path.exists('./Inputs/saved/'+ch+'list')):
                    
                    for a in listsaved:
                        if a['id']==int(ch):
                            listev=a
                            break
                        
                    for d in range(0,listev['days']):
                        shutil.copy2('./Inputs/saved/'+ch+'list/UserMovementsListEvents_'+str(d)+'.txt', './Inputs/Default/Mobility/chosen_list/UserMovementsListEvents_'+str(d)+'.txt')
                    shutil.copy2('./Inputs/saved/'+ch+'list/CoordinatesAntennas.txt', './Inputs/Default/Mobility/chosen_list/CoordinatesAntennas.txt')
                    shutil.copy2('./Inputs/saved/'+ch+'list/route_usr_1day_0.html','/var/www/html/CrowdSenSim/route_usr_1day_0.html')
                    shutil.copy2('./Inputs/saved/'+ch+'list/Setup.txt','./Inputs/Default/Mobility/chosen_list/Setup.txt')
                    
                else:
                    print ("List not present, please try again.\n")
                    
                    for it in listsaved:
                        
                        if it['id']==int(ch):
                            listsaved.remove(it)                     
                    menu_actions['menu1']()
            
            else:
                print ("Invalid o, please try again.\n")
                menu_actions['menu1']()
        
        else:
            print ("Invalid insertion, please try again.\n")
            menu_actions['menu1']()    

    return

# Menu 2
def menu2():
    print ("Create new list of events !\n")
    print ("Insert the name of the city!\n")
    name = input(" >>  ")
    global namecity
    namecity=name
    return  namecity

# Back to main menu
def back():
    menu_actions['main_menu']()

# Exit program
def manage():
    global listsaved
    
    print ('Which List do you want to delete ? ')
    for it in listsaved:
        print (it['id'],'.  city: ',it['name'],'  number of user: ',it['us'],' days ',it['days'] )
    print ("b. Back")
    choice = input(" >>  ")
    ch = choice.lower()
    if ch == '' or ch=='b':
        menu_actions['main_menu']()
    else:
        if ch.isdigit():
            
            try:

                shutil.rmtree('./Inputs/saved/'+ch+'list')
            except:
                pass                
                
            for it in listsaved:
                if it['id']==int(ch):
                    listsaved.remove(it) 
                    print ('List Removed')
                    menu_actions['3']()
                    return                   
            
            print ("List not found, please try again.\n")
            menu_actions['main_menu']()
        
        else:
            print ("Invalid insertion, please try again.\n")
            menu_actions['main_menu']()        

def change():
    os.system('clear')
    global ndays
    global usr    
    print ("Number of Users: "+str(usr))
    print ("Number of days: "+str(ndays))
    print ("\nPlease choose what you want to change:\n")
    print ("1. Change number of USERS")
    print ("2. Change number of DAYS")
    print ("b. Back")

    choice = input(" >>  ")
    ch = choice.lower()
    if ch == '' or ch=='b':
        menu_actions['main_menu']()
        return
    
    if(ch=='1'):
        chusr = input("insert number users >>  ")
        if(chusr.isdigit()):
            usr=int(chusr)
        else:
            print ('wrong insertion,please retry\n')
            menu_actions['4']()
    if(ch=='2'):
        chdays = input("insert number days >>  ")
        if(chdays.isdigit()):
            ndays=int(chdays)
        else:
            print ('wrong insertion,please retry\n')
            menu_actions['4']()        
        
    menu_actions['4']()        

def quit():
    raise SystemExit

# =======================
#    MENUS DEFINITIONS
# =======================

# Menu definition
menu_actions = {
    'main_menu': main_menu,
    '1': menu1,
    '2': menu2,
    'b': back,
    '3': manage,
    '4': change,
    '0': quit,
}




