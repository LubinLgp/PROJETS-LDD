from modules.mixins.graph.architecture import *

class Matrix(Architecture):
    def graph_from_adjacency_matrix(self, matrix: list):  # -> open_digraph
        """
        function to create a multigraph from a matrix
        @param matrix (<int <list > >list): the matrix of the graph
        @return (open_digraph) : a multigraph from a matrix
        """
        n = len(matrix)
        for i in range(n):
            self.add_node("", {}, {})
        for i in range(n):
            for j in range(n):
                multiplicity = matrix[i][j]
                for k in range(multiplicity):
                    self.add_edge(i, j)
        ids = self.get_node_ids()
        for id in ids:
            self.remove_isolated(id)

    @classmethod
    def random(cls, n: int, bound: int, inputs=0, outputs=0, loop_free=False, DAG=False, oriented=False,
               undirected=False):
        """
        function to generate a random graphG according to the constraints given by the user
        @param n (int) : the number of nodes
        @param bound (int): the upper bound of the interval
        @param inputs (int) : the number of inputs nodes
        @param outputs (int) : the number of outputs nodes
        @param loop_free (bool) : specify if a graph can have edges between the same node
        @param DAG (bool) : specify if it's a DAG
        @param oriented (bool) : specify if it's an oriented graph
        @param undirected (bool) : specify if it's a non directed graph
        @returns a graph with the requested options
        """

        if n==0 :
            raise ValueError("Générer un graphe vide n'est pas possible, utilisez plutôt open_digraph.empty()")
        if undirected:
            if DAG or oriented:
                raise ValueError(
                    "Un graphe ne peut pas être non dirigé et dirigé en même temps")
            elif loop_free:
                M = random_int_matrix(n, bound, null_diag=False)
            else:
                M = random_int_matrix(n, bound, null_diag=True)
        elif DAG:
            if loop_free:
                raise ValueError(
                    "Un graphe acyclique ne peut pas avoir des arêtes entre un même noeud")
            else:
                M = random_triangular_int_matrix(n, bound, null_diag=True)
        elif oriented:
            if loop_free:
                M = random_oriented_int_matrix(n, bound, null_diag=False)
            else:
                M = random_oriented_int_matrix(n, bound, null_diag=True)
        elif loop_free:
            M = random_int_matrix(n, bound, null_diag=False)
        else:
            M = random_int_matrix(n, bound, null_diag=True)
        G1 = cls.empty()
        G1.graph_from_adjacency_matrix(M)
        L = G1.get_node_ids()
        if len(L)==0 :
            L.append(G1.add_node())

        for i in range(inputs):
            c = random.choice(L)
            G1.add_input_node(c)
        for j in range(outputs):
            c = random.choice(L)
            G1.add_output_node(c)

        return cls(G1.get_input_ids(), G1.get_output_ids(), G1.get_nodes())
    
    def from_id_to_dict(self) -> dict:
        """
        @returns a dictionary, associating, to each node id, a unique integer
        """
        D = {}
        ids = self.get_node_ids()
        for i, id in enumerate(ids):
            D[i] = id
        return D


    def adjacency_matrix(self) -> list:
        """
        @returns an adjacency matrix for the graph
        """

        D = self.from_id_to_dict()
        n = len(D)
        T = list()
        for i in range(n):
            ligne = list()
            id = D[i]
            children_id = (self.get_node_by_id(id)).get_children()
            for j in range(n):
                if D[j] in children_id:
                    ligne.append(children_id[D[j]])
                else:
                    ligne.append(0)
            T.append(ligne)
        return T