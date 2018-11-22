import random
import uuid

algList = ['1', '2', '3']
elements = ["H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt"]


def generate(n):
  entrada = open("entrada.in",'w')

  for i in range(1,n+1):
    UUID = str(uuid.uuid4())
    algId = random.choice(algList)
    
    if algId == '3':
      params = random.choice(elements)
    else:
      params = str(random.randint(10,10000))

    entrada.write(','.join([UUID,algId,params+'\n']))
  entrada.close()
  
generate(100)