from modules.open_digraph import *

# Pour plus de tests sur les méthodes [calcul] et [verificationHamming] veuillez regarder le fichier
# open_digraph_test.py

# ATTENTION : L'exécution de chaque commande va faire s'ouvrir un peu moins d'une dizaine de page Internet représentant
# l'état du graphique en cours de transformation. Veuillez exécuter les commandes l'une après l'autre.

# Point 2
# Nous voulons effectuer l'addition de 11 et 7 avec une retenue sur des registres de taille 2**2, donc sur adder(2)
# On écrit 11 et 7 en binaire sur 2**2 bits avec la retenue : 11 = 1011, 7 = 0111, 1 = 1.
# Le nombre binaire obtenu est 101101111, ce qui vaut 367 en décimal.
# De plus, on sait que 11+7+1=19, qui vaut en décimal 10011.
# Donc l'appel calcul(2, 367) effectue l'addition de 11 et 7 avec une retenue, et écrit le résultat en binaire,
# soit normalement 10011

open_digraph.bool_circ.calcul(2, 367, etape_int=True)

# Point 4
# Nous voulons tester la composition d'un encodeur et d'un décodeur de Hamming avec en entrée un
# entier compris entre 0 et 15 (inclus) tiré aléatoirement

# open_digraph.bool_circ.verificationHamming([],showingMode = True) (A DÉCOMMENTER)
