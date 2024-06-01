from modules import node
from modules.mixins.graph.architecture import *

class Comp_Chemin(Architecture):
    def is_cyclic(self) -> bool:
        """ Revoie vrai si le graph [self] est cyclique et non sinon"""
        G = self.copy()  # car les instructions suivantes vont dÃ©truire le graph

        def is_cyclic_AUX(G):  # permet de faire une seule copie de [self] et non pas une Ã  chaque appel
            nodes = G.get_nodes()
            if len(nodes) == 0:
                return False
            for node in nodes:
                if len(node.get_children()) == 0:
                    G.remove_node_by_id(node.get_id())  # il n'existe pas deux noeuds identiques
                    return G.is_cyclic()
            return True

        return is_cyclic_AUX(G)

    
    def iparallel(self, g):
        """
        method to modify the graph (self) by adding in parallel another graph
        @param g (open_digraph): a graph
        """

        if (self == g):
            g = g.copy()  # car on veut que [g] soit inchangé

        m = min(self.get_node_ids(),default=0)
        M = max(g.get_node_ids(),default=0)
        self.shift_indices(M - m + 1)
        self.set_input_ids(g.get_input_ids() + self.get_input_ids())
        self.set_output_ids(g.get_output_ids()+self.get_output_ids())
        self.nodes = {node.id: node for node in g.get_nodes() + self.get_nodes()}
        self.next_id = self.init_next_id()


    def parallel(self, g):  # -> open_digraph
        """
        method to create a new graph by adding in parallel two graphs
        @param g (open_digraph): a graph
        @return (open_digraph) : a new graph which is the parallel
        """
        newG = self.copy()
        newG.iparallel(g)
        return newG


    def icompose(self, f):
        """
        method which performs the sequential composition of two graphs
        @param f (open_digraph): a graph
        """
        if not (len(f.get_output_ids()) == len(self.get_input_ids())):
            raise ValueError("the numbers of inputs from self and outputs from f do not match")
        inputs = self.get_nodes_by_ids(self.get_input_ids())
        n = len(inputs)
        f = f.copy()
        outputs = f.get_nodes_by_ids(f.get_output_ids())
        self.iparallel(f)
        for i in range(n):
            input = inputs[i]
            output = outputs[i]
            self.add_edge(output.get_parent_ids()[0], input.get_children_ids()[0])
            self.remove_node_by_id(output.get_id())
            self.remove_node_by_id(input.get_id())


    def compose2(self, f):  # -> open_digraph
        """
        method to create a new graph by making a sequential composition of two graphs
        @param f (open_digraph): a graph
        @return (open_digraph) : a new graph which is the sequential composition
        """
        newG = self.copy()
        newG.icompose(f)
        return newG

    @classmethod
    def compose(cls, g1, g):
        """
        Return a new graph that is the composition of self and g, without modifying them.

        Parameters:
        - g: graph

        Returns:
        - open_digraph: A new graph representing the composition of self and g.
        """
        g2 = g1.copy()
        g2.icompose(g)
        return g2

    
    def Dijsktra(self, src : node, direction=None, tgt=None):
        """ Renvoie un tuple composé d'un dictionnaire composé de la distance entre le point src et les points connexes à ce point, et
        un dictionnaire composé en clés des noeuds parcourus ayant un noeud précédent avec
        comme clé ce noeud
        ex : Si le chemin est n1-n2-n3 en partant du point n1, le premier dictionnaire est {n1:0, n2:1, n3:2}
        et le deuxième dictionnaire = {n2 : n1, n3 : n2}
        Args:
            src : Noeud destinataire
            direction :  None -> On cherche dans les parents et les enfants
                         1 -> On cherche sur les enfants uniquement
                         -1 -> On cherche sur les parents uniquement
        Returns:src
            (distance, dictionnaire)
        """
        Q = [src]
        dist = {src : 0}
        prev = {}
        while Q != [] :
            u, dist_min = Q[0], dist[Q[0]]
            for i in range(len(Q)):
                if dist[Q[i]] < dist_min :
                    u = Q[i]
                    print(u)
                    dist_min = dist[Q[i]]
            Q.remove(u)
            if u == tgt :
                return (dist, prev)
            if direction == None :
                L = []
                D = u.get_parent_ids() + u.get_children_ids()
                for e in D :
                    if not e in L :
                        L.append(e)
            elif direction == 1 :
                L = list(u.get_children())
            elif direction == -1 :
                L = list(u.get_parents())
            else :
                raise ValueError("Valeur de la direction incorrecte")
            neighbours = [self.get_node_by_id(n) for n in L]
            for v in neighbours :
                if not v in dist.keys() :
                    Q.append(v)
                if (not v in dist.keys()) or (dist[v] > dist[u] + 1):
                    dist[v] = dist[u] + 1
                    prev[v] = u
        return (dist, prev)

    def shortest_path(self, src : node , tgt : node):
        dist,prev=self.Dijkstra(src,tgt=tgt)
        if not tgt in dist.keys() :
            raise ValueError("tgt isn't connected to the src")
        else :
            chemin={}
            currentNode=tgt
            while currentNode!=src:
                chemin[currentNode]=prev[currentNode]
                currentNode=prev[currentNode]
            return chemin

    def ancetre_commun(self, node1 : node, node2 : node):
        d1,p1=self.Dijkstra(node1,direction=-1)
        d2,p2=self.Dijkstra(node2,direction=-1)
        common={}
        for d in d1.keys():
            if d in d2.keys():
                common[d]=d1[d],d2[d]


    @classmethod
    def identity(cls, n: int):  # ->open_digraph
        """
        function to create a graph which represent the identity over n children
        @param n (int) : number of children
        @returns (open_digraph) : a graph representing the identity of n children
        """

        if (n == 1):
            i = node(0, "", {}, {2: 1})
            o = node(2, "", {0: 1}, {})
            return cls([0], [2], [i, o])
        else:
            return cls.identity(n - 1).parallel(cls.identity(1))

    @classmethod
    def identity_list(cls, n : list):
        """Renvoie l'identité des n inputs donnés en arguments sans noeuds intermédiaires entre les inputs et outputs"""
        g = cls([e.get_id() for e in n], [], n)
        for i in range(len(n)):
            id_i = g.new_id()
            out = node(id_i, "", {}, {})
            g.nodes[id_i] = out
            g.outputs.append(id_i)
            g.add_edge(n[i].get_id(), id_i)
        return g

    def connected_components(self):  # -> (int * <int : int> dict)
        """
        function to count the number of connected components in a graph
        @return (int * <int : int> dict) : the number of connected components of the graph and a dictionary that
        associates an int to each node id in the graph
        """
        ids = set(self.get_node_ids())
        d = dict()
        nbComponents = -1
        def auxiliary_function(id : int):
            """
            auxiliary function traversing recursively over the parents and children of the node and deleting the id already seen
            @param id (int) : the id of a node
            """
            if id in ids:
                try:
                    ids.remove(id)
                except ValueError:
                    return
                d[id] = nbComponents
                node = self.get_node_by_id(id)
                for child_id in node.get_children_ids():
                    auxiliary_function(child_id)
                for parent_id in node.get_parent_ids():
                    auxiliary_function(parent_id)

        while (len(ids) != 0) :
            nbComponents += 1
            f_node = next(iter(ids))
            auxiliary_function(f_node)
        return (nbComponents, d)


    def get_connected_components(self) -> list:
        """
        method to get connected components of the starting graph
        @returns (<open_digraph> list) : a list graph, each corresponding to a connected component of the
        starting graph
        """

        res = list()
        n, d = self.connected_components()
        for i in range(n+1):
            nodes = list()
            inputs = list()
            outputs = list()
            for id, v in d.items():
                if v == i:
                    if id in self.inputs:
                        inputs.append(id)
                    if id in self.outputs:
                        outputs.append(id)
                    nodes.append(self.get_node_by_id(id))
            res.append(self.__class__(inputs, outputs, nodes))
        return res
    
    def tri_topologique(self):
        """ Réalise le tri topologique de [self]
        Returns:
            <<int> list> list: Liste des ids des nœuds classés par profondeur
        """
        G = self.copy()
        nodes = G.get_nodes()
        L = []
        while len(nodes) != 0:
            l = []
            for node in nodes:
                if len(node.get_parents()) == 0:
                    l.append(node.get_id())
            L.append(l)
            for node_id in l:
                G.remove_node_by_id(node_id,rem_iso=False)
            nodes = G.get_nodes()
        return L

    def get_node_profondeur(self, id: int) -> int:
        """ Renvoie la profondeur du nœud d'id [id] présent dans [self]"""
        tri = self.tri_topologique()
        p = 0
        n = len(tri)
        for i in range(n):
            if id in tri[i]:
                return p
            else:
                p += 1
        raise ValueError(f"Le noeud d'id {id} n'est pas dans le graphe")

    def get_profondeur(self):
        """ Renvoie la profondeur du graph [self]"""
        return len(self.tri_topologique())

    def calcul_chemin_distance(self, u: node, v: node):
            """
                Retourne la distance maximale entre deux noeuds donnés en parametre
                ainsi que le chemin de distance maximale entre ces derniers
                inputs:
                    u, v: noeuds
                outputs :
                    un double (d_max: int, chemin_max: list) où d_max est un
                    entier representant la distance max entre [u] et [v]; et
                    chemin_max étant une liste qui contient les [ids] des
                    noeuds parcourus (de façon ordonnée et chronologique) à
                    partir de [u] inclus vers [v] inclus.
            """

            if self.is_cyclic():
                raise ValueError("Erreur: Le graphe est cyclic\n")
            else:
                nodes = self.get_nodes()
                if u not in nodes :
                    raise ValueError("Le noeud dont l'id est {} n'est pas dans le graphe".format(u.get_id()))
                if v not in nodes :
                    raise ValueError("Le noeud dont l'id est {} n'est pas dans le graphe".format(v.get_id()))
                t = self.tri_topologique()
                k = self.get_node_profondeur(u.get_id()) + 1
                dist = {u.get_id() : 0}
                prev = {u.get_id(): u.get_id()}
                found = False
                while (not found and k < len(t)):
                    for w in t[k]:
                        m = 0
                        for p in self.get_node_by_id(w).get_parent_ids():
                            if p in dist:
                                if dist[p] >= m:
                                    dist[w] = dist[p] + 1
                                    prev[w] = p
                                    m = dist[p]
                        if w == v.get_id():
                            found = True
                            i = v.get_id()
                            l = [i]
                            while i != u.get_id():
                                i = prev[i]
                                l.insert(0, i)
                            return max(dist.values()), l
                    k = k + 1
                if not found:
                    raise ValueError("Il n'existe aucun chemin de n{} vers n{}".format(u.get_id(), v.get_id()))