import catch
import fre

def get_ip(pack):
    ###x,y = pack.index('Source Address: '),pack.index('Destination Address')
    #x=x+17
    
    #z = pack[x:y]
    #print(y)
    ###
    
    
    
    
    #print(pack.ip.src)
    return pack.ip.src
    
    
def run(ip):
    fre.block_ip(ip)
    
    
def blo(pack):
    run(get_ip(pack))
    