import random
import uuid

algList = ['1', '2', '3']
numbers = ['1','2','3','4','5','6','7','8','9']
numbers0 = ['1','2','3','4','5','6','7','8','9','0']
elements = ["H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt"]


def generate(n):
  entrada = open("entrada.in",'w')

  for i in range(1,n+1):
    UUID = str(uuid.uuid4())
    algId = random.choice(algList)
    
    if algId == '1':
      l = random.randint(2,18)
      params = random.choice(numbers)
      params += ''.join(random.choices(numbers0, k=l-1))
    elif algId == '2':
      params = str(random.randint(100,100000000))
    else:
      params = random.choice(elements)

    entrada.write(','.join([UUID,algId,params+'\n']))
  entrada.close()
  
generate(10000)