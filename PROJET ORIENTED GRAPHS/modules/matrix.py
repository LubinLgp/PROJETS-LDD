import sys
import os
root = os.path.normpath(os.path.join(__file__, './../..'))
sys.path.append(root)# allows us to fetch files from the project root
from random import randint

def random_int_list(n : int, bound : int) -> list :
    """
        function to generate a list of size [n] of integers within [0,bound].
        @param n (int): the size of the list
        @param bound (int): the upper bound of the interval
        @returns <int> list: a list of size n of integers within [0,bound]
        """
    l = []
    for i in range(n):
        l.append(randint(0, bound))
    return l


def random_int_matrix(n : int ,bound : int , null_diag = True) -> list :
    """
    function to generate a square matrix of size n of integers whithin [0,bound]
    @param n (int): the size of the matrix
    @param bound (int): the upper bound of the interval
    @param null_diag (bool) (optional) : specify if the diagonal is null
    @returns < <int> list> >list: square matrix of size n
    """

    T = list()
    for i in range(n):
        ligne = list()
        for j in range(n):
            if(null_diag and i==j):
                ligne.append(0)
            else :
                ligne.append(randint(0,bound))
        T.append(ligne)
    return T

def random_symetric_int_matrix(n :int, bound : int ,null_diag=True) -> list :
    """
    function to generate a symetric matrix of size n x n of integers whithin [0,bound]
    @param n (int): the size of the matrix
    @param bound (int): the upper bound of the interval
    @param null_diag (bool) (optional) : specify if the diagonal is null
    @returns < <int> list> >list: symetric matrix of size n
    """
    T = []
    for i in range(n):
        ligne = []
        for j in range(n):
            if i > j:
                ligne.append(T[j][i])
            elif (i == j and null_diag):
                ligne.append(0)
            else:
                ligne.append(randint(0, bound))
        T.append(ligne)
    return T


def empty_matrix(n):
    """
    function to generate a zero matrix of size n x n
    @param n (int): the size of the square matrix
    @returns < <int> list> >list: Square matrix of size n
    """
    return [[0 for _ in range(n)] for _ in range(n)]


def random_oriented_int_matrix(n: int, bound: int, null_diag=True):
    """
    function to generate an oriented matrix of size n x n of integers whithin [0,bound]
    @param n (int): the size of the matrix
    @param bound (int): the upper bound of the interval
    @param null_diag (bool) (optional) : specify if the diagonal is null
    @returns < <int> list> >list: an oriented matrix of size n x n
    """

    M = empty_matrix(n)
    for i in range(n):
        for j in range(i, n):
            if (i != j):
                x = randint(0, bound)
                if randint(0, 1) == 0:
                    M[i][j] = x
                else:
                    M[j][i] = x
            else:
                if (null_diag):
                    M[i][i] = 0
                else:
                    M[i][i] = randint(0, bound)
    return M


def random_triangular_int_matrix(n : int , bound : int , null_diag = True) -> list :
    """
    function to generate random DAGs with integers whithin [0,bound]
    @param n (int): the size of the matrix
    @param bound (int): the upper bound of the interval
    @param null_diag (bool) (optional) : specify if the diagonal is null
    @returns < <int> list> >list: a matrix of size n x n
    """
    matrix = empty_matrix(n)
    for i in range(n):
        for j in range(i, n):
            matrix[i][j] = randint(0, bound)
    if null_diag:
        for i in range(n):
            matrix[i][i] = 0
    return matrix
