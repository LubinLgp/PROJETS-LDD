import random

from modules.matrix import *
from modules.node import *
from modules.mixins.graph.architecture import *
from modules.mixins.graph.affichage import *
from modules.mixins.graph.comp_chemin import *
from modules.mixins.graph.matrix_graph import *
from random import choice


class open_digraph(Affichage, Comp_Chemin, Matrix):  # for open directed graph
    def __init__(self, inputs, outputs, nodes):
        '''
        inputs: int list; the ids of the input nodes
        outputs: int list; the ids of the œoutput nodes
        nodes: node iter;
        '''
        self.inputs = inputs
        self.outputs = outputs
        self.nodes = {node.id: node for node in nodes}  # self.nodes: <int,node> dict
        self.next_id = self.init_next_id()  # indice du prochain noeud ajouté

    # Native methods
    def __str__(self) -> str:
        return "Digraph(\n input = {} \n output = {} \n nodes = {} )".format(self.inputs, self.outputs, self.nodes)

    def __repr__(self) -> str:
        return self.__str__()

    def __getitem__(self, id: int):
        return self.get_node_by_id(id)

    # Methods
    class bool_circ(Matrix):
        def __init__(self, g):
            """ Initialise une instance de 'bool_circ'
            Args:
                g (open_dipgraph): une instance d'open_digraph valide
            Raises:
                ValueError: si le graph [g] n'est pas une instante d'open_digraph
                ValueError: si le graph [g] n'est pas un open_digraph valide
            """
            if not isinstance(g, open_digraph):
                raise ValueError("L'agument [g] doit être une instance de open_digraph")
            if not g.is_well_formed():
                raise ValueError("L'open digraph [g] donné en argument est mal formé")
            self.graph = g
            self.assert_is_well_formed()

        def get_graph(self):
            return self.graph

        def is_well_formed(self):  # -> (int*int)
            """ Renvoie l'entier 0 si le graphe est valide ou un entier entre 1 et 6
            selon l'erreur sinon. Peut renvoyer l'id du nœud auquel se trouve l'erreur.
            Returns:
                (int*int) tuple : La première composante est l'encodage de l'erreur et la
                deuxième composante est l'id du nœud problématique
            from bool_circ
            """
            if not self.graph.is_well_formed() or self.graph.is_cyclic():
                return (1, None)
            list_label = ["0", "1", "", "&", "|", "^", "~"]
            for node in self.graph.get_nodes():
                if node.get_id() in self.graph.get_input_ids() or node.get_id() in self.graph.get_output_ids():
                    continue
                label = node.get_label()
                if not label in list_label:
                    return (2, node.get_id())
                if label == "" and node.indegree() != 1:
                    return (3, node.get_id())
                if label == "&" and (node.outdegree() != 1 or node.indegree() < 2):
                    return (4, node.get_id())
                if label == "|" and (node.outdegree() != 1 or node.indegree() < 2):
                    return (5, node.get_id())
                if label == "^" and (node.outdegree() != 1 or node.indegree() < 2):
                    return (6, node.get_id())
                if label == '~' and (node.outdegree() != 1 or node.indegree() != 1):
                    return (7, node.get_id())
                if (label == "0" or label == "1") and (node.indegree() != 0 or node.outdegree() != 1):
                    return (8, node.get_id())
            return (0, None)

        def is_well_formed_boolean(self) -> bool:
            """ Renvoie True si le graph est valide ou False sinon from bool_circ"""
            return self.is_well_formed()[0] == 0

        def assert_is_well_formed(self):
            """ Renvoie une erreur si le graph n'est pas formé correctement from bool_circ"""
            err, id = self.is_well_formed()
            errors = {1: ValueError("Le graph doit être acyclique bien formé from is_well_formed"),
                      2: ValueError(f"Le label du noeud d'id {id} n'a pas un label valide from is_well_formed"),
                      3: ValueError(f"Le noeud 'copie' d'id {id} n'a pas un degré entrant valide from is_well_formed"),
                      4: ValueError(f"Le noeud 'et' d'id {id} n'a pas un degré entrant ou sortant valide from "
                                    f"is_well_formed"),
                      5: ValueError(f"Le noeud 'ou' d'id {id} n'a pas un degré entrant ou sortant valide from "
                                    f"is_well_formed"),
                      6: ValueError(f"Le noeud 'ou exclusif' d'id {id} n'a pas un degré sortant ou entrant valide from "
                                    f"s_well_formed"),
                      7: ValueError(f"Le noeud 'non' d'id {id} n'a pas un degré entrant ou sortant valide from "
                                    f"is_well_formed"),
                      8: ValueError(f"Le noeud 'constante' d'id {id} n'a pas un degré entrant ou sortant valide from "
                                    f"is_well_formed")
                      }
            try:
                assert err == 0
            except(AssertionError):
                raise errors[err]

        @classmethod
        def parse_parentheses(cls, *args):  # -> (open_digraph, <int> list)
            """_summary_
            Args:
                *args : séquence de chaînes de caractères
            Returns :
                Un tuple formé du graphe formé et de la liste des ids des inputs
            """
            L = []
            for ecriture in args:
                G = open_digraph.empty()
                current_node = G.add_node()
                G.add_output_node(current_node)
                s2 = ''
                for c in ecriture:
                    if c == '(':
                        G[current_node].set_label(G[current_node].get_label() + s2)
                        current_node = G.add_node(children={current_node: 1})
                        s2 = ''
                    elif c == ')':
                        G[current_node].set_label(G[current_node].get_label() + s2)
                        current_node = G[current_node].get_children_ids()[0]
                        s2 = ""
                    else:
                        s2 += c
                L.append(G)
            G = L[0]
            for i in range(1, len(L)):
                G = G.parallel(L[i])
            list_label = ["0", "1", "&", "|", "^", "~", ""]
            D = {}
            nodes = G.get_node_ids()
            for id in nodes:
                lab = G.get_node_by_id(id).get_label()
                if lab not in list_label:
                    if lab not in D.keys():
                        D[lab] = [id]
                    else:
                        D[lab].append(id)
            L = []
            for l in D.keys():
                if len(D[l]) != 1:
                    id_fusion = G.fusion_nodes(D[l], label='')
                    input_node = G.add_input_node(id_fusion, label=l)
                    L.append(input_node)
                else:
                    input_node = G.add_input_node(D[l][0], label=l)
                    G.get_node_by_id(D[l][0]).set_label("")
                    L.append(input_node)

            return (G, L)

        @classmethod
        def bool_circ(cls, size: int, inputs=1, outputs=1):  # -> open_digraph.bool_circ

            """ Fonction qui retourne un circuit booléen bien-formé.
            Prend en arguments:
                - size (int) : le nombre de nœuds initiaux avant la transformation
                - inputs (int) : le nombre d'entrées qui doit être bien-sûr >= 1
                - outputs: le nombre de sorties qui doit être bien-sûr >= 1
        Returns :
                - Un circuit booléen bien-formé.
                PS: il arrive que le nombre de nœuds finaux soit inférieur à size.
                On a prit le soin de supprimer des nœuds isolés.
        Raise :
                - Une erreur de valeurs lorsque inputs < 1 ou outputs < 1 """

            if outputs < 1 or inputs < 1:
                raise ValueError("inputs (outputs) doit être supérieur ou égal à 1")

            G = open_digraph.random(size, 1, inputs=0, outputs=0, loop_free=False, DAG=True, oriented=False,
                                    undirected=False)

            for n in G.get_nodes():
                if len(n.get_parent_ids()) == 0:
                    G.add_input_node(n.get_id())
                if len(n.get_children_ids()) == 0:
                    G.add_output_node(n.get_id())

            inpts = G.get_input_ids()
            outpts = G.get_output_ids()
            while len(inpts) != inputs or len(outpts) != outputs:
                if len(inpts) > inputs:
                    i0 = choice(inpts)
                    i1 = choice([i for i in inpts if i != i0])
                    new_input = G.add_node()
                    for c in G.get_node_by_id(i0).get_children():
                        G.add_edge(new_input, c)
                    for c in G.get_node_by_id(i1).get_children():
                        G.add_edge(new_input, c)
                    G.add_input_node(new_input)
                    G.remove_node_by_id(i0)
                    G.remove_node_by_id(i1)
                elif len(inpts) < inputs:
                    nds = [n for n in G.get_node_ids() if (n not in G.get_input_ids()) and n not in G.get_output_ids()]
                    G.add_input_node(choice(nds))

                if len(outpts) > outputs:
                    o0 = choice(outpts)
                    o1 = choice([o for o in outpts if o != o0])
                    new_output = G.add_node()
                    for p in G.get_node_by_id(o0).get_parents():
                        G.add_edge(p, new_output)
                    for p in G.get_node_by_id(o1).get_parents():
                        G.add_edge(p, new_output)
                    G.add_output_node(new_output)
                    G.remove_node_by_id(o0)
                    G.remove_node_by_id(o1)

                elif len(outpts) < outputs:
                    nds = [n for n in G.get_node_ids() if (n not in G.get_input_ids()) and n not in G.get_output_ids()]
                    G.add_output_node(choice(nds))

                inpts = G.get_input_ids()
                outpts = G.get_output_ids()
            for n in G.get_nodes():
                i_deg = n.indegree()
                o_deg = n.outdegree()
                if i_deg == 1 and o_deg == 1:
                    n.set_label("~")
                elif i_deg > 1:
                    n.set_label(choice(["&", "^", "|"]))
                    if o_deg > 1:

                        uid = G.add_node(label=n.get_label(), parents={}, children={})
                        for p in n.get_parents():
                            G.add_edge(p, uid)
                        vid = G.add_node(parents={uid: 1})
                        for c in n.get_children():
                            G.add_edge(vid, c)
                        G.remove_node_by_id(n.get_id(), rem_iso=False)
            return cls(G)

        @classmethod
        def adder0(cls):  # -> open_digraph.bool_circ
            """Donne le circuit booléen avec comme graphe, le graphe représentant le circuit pour l'addition de 2 registres
            de taille 1 avec une retenue"""
            id = 0
            i0 = node(-1, "", {}, {})
            i1 = node(-2, "", {}, {})
            i2 = node(-3, "", {}, {})
            c0 = node(id, "", {}, {id + 2: 1, id + 5: 1})
            c1 = node(id + 1, "", {}, {id + 2: 1, id + 5: 1})
            n1 = node(id + 2, "^", {id: 1, id + 1: 1}, {id + 3: 1})
            c3 = node(id + 3, "", {id + 2: 1}, {id + 6: 1, id + 7: 1})
            c2 = node(id + 4, "", {}, {id + 6: 1, id + 7: 1})
            n2 = node(id + 5, "&", {id: 1, id + 1: 1}, {id + 8: 1})
            n3 = node(id + 6, "&", {id + 3: 1, id + 4: 1}, {id + 8: 1})
            n4 = node(id + 7, "^", {id + 3: 1, id + 4: 1}, {})
            n5 = node(id + 8, "|", {id + 5: 1, id + 6: 1}, {})
            o0 = node(id + 9, "", {}, {})
            o1 = node(id + 10, "", {}, {})
            G = open_digraph([i0.get_id(), i1.get_id(), i2.get_id()], [id + 9, id + 10],
                             [i0, i1, c0, c1, c2, c3, n1, n2, i2, n3, n4, n5, o0, o1])
            G.add_edge(i0.get_id(), id)
            G.add_edge(i1.get_id(), id + 1)
            G.add_edge(i2.get_id(), id + 4)
            G.add_edge(id + 8, id + 9)
            G.add_edge(id + 7, id + 10)
            id0, id1 = G.get_output_ids()[0], G.get_output_ids()[1]

            for id in G.get_output_ids():
                if G.get_node_by_id(G.get_node_by_id(id).get_parent_ids()[0]) == "|":
                    id0 = id
                else:
                    id1 = id
            G.set_output_ids([id0, id1])

            return open_digraph.bool_circ(G)

        @classmethod
        def adder(cls, n: int):  # -> open_digraph.bool_circ
            """Renvoie le circuit booléen de l'addition de 2 registres avec comme argument la puissance de 2 correspondant
            à la taille de ces registres avec une retenue"""
            if n == 0:
                return open_digraph.bool_circ.adder0()
            else:
                G1 = open_digraph.identity(2 ** n)
                G2 = open_digraph.bool_circ.adder(n - 1).get_graph()
                G_inter1 = G2.parallel(G1)

                G3 = open_digraph.bool_circ.adder(n - 1).get_graph()
                G4 = open_digraph.identity(2 ** (n - 1))
                G_inter2 = G4.parallel(G3)

                Res = G_inter2.compose2(G_inter1)
                inputs = Res.get_input_ids()
                i1 = inputs[0:2 ** (n - 1)]
                i2 = inputs[2 ** (n - 1):2 ** (n)]
                i3 = inputs[2 ** n:3 * (2 ** (n - 1))]
                i4 = inputs[3 * (2 ** (n - 1)):2 ** (n + 1)]
                c = inputs[2 ** (n + 1)]
                if not isinstance(i1, list):
                    i1 = list(i1)
                if not isinstance(i2, list):
                    i2 = list(i2)
                if not isinstance(i3, list):
                    i3 = list(i3)
                if not isinstance(i4, list):
                    i4 = list(i4)
                if not isinstance(c, list):
                    c = [c]
                L = i1 + i3 + i2 + i4 + c
                Res.set_input_ids(L)

                return open_digraph.bool_circ(Res)

        @classmethod
        def half_adder(cls, n: int):  # -> open_digraph.bool_circ
            """Effecute l'addition de 2 registres de taille 2**n sans retenu (l'addition classique)"""
            B = open_digraph.bool_circ.adder(n)
            B.graph.get_node_by_id(B.get_graph().get_input_ids()[len(B.get_graph().get_input_ids()) - 1]).set_label("0")
            return B

        @classmethod
        def from_int_to_registre(cls, n: int, taille_registre=8):  # -> open_digraph.bool_circ
            """Renvoie le graphe composé en inputs de l'entier [n] écrit en binaire sur taille_registre bits.
            La fonction lève une exception si l'écriture de l'entier n en binaire est supérieure à la taille du registre"""
            s = bin(n)[2:]
            if len(s) > taille_registre:
                raise ValueError("Taille du registre trop faible")
            else:
                L = []
                diff = -1
                for i in range(len(s)):
                    L.append(int(s[i]))
                if taille_registre - len(s) != 0:
                    diff += taille_registre - len(s)
                T = [0 for i in range(diff + 1)]
                L = T + L
                new_id = 0
                for i in range(len(L)):
                    L[i] = node(new_id, str(L[i]), {}, {})
                    new_id += 1
                G = open_digraph.identity_list(L)
                return open_digraph.bool_circ(G)

        def simpl_COPY(self, id1: int, id2: int):
            """Effectue la simplification dans le cas de la copie.
            Args :
                id1 : l'id d'un noeud de label 0 ou 1
                id2 : l'id d'un noeud copie qui est le fils direct du noeud d'id id1
            Raise :
                Lève une exception si le noeud d'id id1 n'a pas un label valant 0 ou 1
                ou si le noeud d'id id2 n'est pas un noeud copie"""
            n1 = self.graph.get_node_by_id(id1)
            n2 = self.graph.get_node_by_id(id2)
            if n2.get_label() == "":
                if n1.get_label() == "0":
                    L = n2.get_children_ids()
                    for id in L:
                        self.graph.add_node('0', {}, {id: 1})
                    self.graph.remove_node_by_id(id1, rem_iso=False)
                    self.graph.remove_node_by_id(id2)
                elif n1.get_label() == "1":
                    L = n2.get_children_ids()
                    for id in L:
                        self.graph.add_node('1', {}, {id: 1})
                    self.graph.remove_node_by_id(id1, rem_iso=False)
                    self.graph.remove_node_by_id(id2)
                else:
                    raise ValueError("Le premier id n'est pas un id d'un 0 ou d'un 1")
            else:
                raise ValueError("Le deuxième id n'est pas un id d'un noeud de copie")

        def simpl_NON(self, id1: int, id2: int):
            """Effectue la simplification dans le cas de la négation.
            Args :
                id1 : l'id d'un noeud de label 0 ou 1
                id2 : l'id d'un noeud non qui est le fils direct du noeud d'id id1
            Raise :
                Lève une exception si le noeud d'id id1 n'a pas un label valant 0 ou 1
                ou si le noeud d'id id2 n'est pas un noeud copie"""
            n1 = self.graph.get_node_by_id(id1)
            n2 = self.graph.get_node_by_id(id2)
            if n2.get_label() == "~":
                if n1.get_label() == "0":
                    self.graph.remove_node_by_id(id1)
                    n2.set_label("1")
                elif n1.get_label() == "1":
                    self.graph.remove_node_by_id(id1)
                    n2.set_label("0")
                else:
                    raise ValueError("Le premier id n'est pas un id d'un 0 ou d'un 1")
            else:
                raise ValueError("Le deuxième id n'est pas un id d'un noeud de négation")

        def simpl_ET(self, id1: int, id2: int):
            """Effectue la simplification dans le cas du ET logique.
            Args :
                id1 : l'id d'un noeud de label 0 ou 1
                id2 : l'id d'un noeud ET qui est le fils direct du noeud d'id id1
            Raise :
                Lève une exception si le noeud d'id id1 n'a pas un label valant 0 ou 1
                ou si le noeud d'id id2 n'est pas un noeud ET"""
            n1 = self.graph.get_node_by_id(id1)
            n2 = self.graph.get_node_by_id(id2)
            if n2.get_label() == "&":
                if n1.get_label() == "0":
                    self.graph.remove_node_by_id(id1)
                    L = n2.get_parent_ids()
                    for id in L:
                        self.graph.remove_parallel_edges(id, id2, rem_iso=False)
                        self.graph.add_node('', {id: 1}, {})
                    n2.set_label('0')
                elif n1.get_label() == "1":
                    self.graph.remove_node_by_id(id1)
                else:
                    raise ValueError("Le premier id n'est pas un id d'un 0 ou d'un 1")
            else:
                raise ValueError("Le deuxième id n'est pas un id d'un noeud ET")

        def simpl_OU(self, id1: int, id2: int):
            """Effectue la simplification dans le cas du OU logique.
            Args :
                id1 : l'id d'un noeud de label 0 ou 1
                id2 : l'id d'un noeud OU qui est le fils direct du noeud d'id id1
            Raise :
                Lève une exception si le noeud d'id id1 n'a pas un label valant 0 ou 1
                ou si le noeud d'id id2 n'est pas un noeud OU"""
            n1 = self.graph.get_node_by_id(id1)
            n2 = self.graph.get_node_by_id(id2)
            if n2.get_label() == "|":
                if n1.get_label() == "0":
                    self.graph.remove_node_by_id(id1)
                elif n1.get_label() == "1":
                    self.graph.remove_node_by_id(id1)
                    L = n2.get_parent_ids()
                    for id in L:
                        self.graph.remove_parallel_edges(id, id2, rem_iso=False)
                        self.graph.add_node('', {id: 1}, {})
                    n2.set_label('1')
                else:
                    raise ValueError("Le premier id n'est pas un id d'un 0 ou d'un 1")
            else:
                raise ValueError("Le deuxième id n'est pas un id d'un noeud OU")

        def simpl_OUEX(self, id1: int, id2: int):
            """Effectue la simplification dans le cas du OU EXCLUSIF.
            Args :
                id1 : l'id d'un noeud de label 0 ou 1
                id2 : l'id d'un noeud OU EXCLUSIF qui est le fils direct du noeud d'id id1
            Raise :
                Lève une exception si le noeud d'id id1 n'a pas un label valant 0 ou 1
                ou si le noeud d'id id2 n'est pas un noeud OU EXCLUSIF"""
            n1 = self.graph.get_node_by_id(id1)
            n2 = self.graph.get_node_by_id(id2)
            if n2.get_label() == "^":
                if n1.get_label() == "0":
                    self.graph.remove_node_by_id(id1)
                elif n1.get_label() == "1":
                    self.graph.remove_node_by_id(id1)
                    id_fils = n2.get_children_ids()
                    if len(id_fils) == 0:
                        self.graph.add_node('~', {id2: 1}, {})
                    else:
                        self.graph.add_node('~', {id2: 1}, {id_fils[0]: 1})
                        self.graph.remove_edge(id2, id_fils[0])

                else:
                    raise ValueError("Le premier id n'est pas un id d'un 0 ou d'un 1")
            else:
                raise ValueError("Le deuxième id n'est pas un id d'un noeud OU Exclusif")

        def simpl_EN(self, id: int):
            """Effectue la simplification dans le cas des éléments neutres.
            Args :
                id1 : l'id d'un noeud de label OU, ET, OU EXCLUSIF
            Raise :
                Lève une exception si le noeud d'id id1 n'a pas un label valant les valeurs décrites ci-dessus
                ou si le noeud d'id id1 a un parent"""
            n = self.graph.get_node_by_id(id)
            if len(n.get_parent_ids()) == 0:
                if n.get_label() == '|' or n.get_label() == '^':
                    n.set_label('0')
                elif n.get_label() == '&':
                    n.set_label('1')
                else:
                    raise ValueError("Le label du noeud est incorrecte")
            else:
                raise ValueError("Le noeud a des parents")

        # ------------------------------------------------------
        def associativite_XOR(self, id1: int, id2: int):
            """
            Applique la règle d'associativité sur deux nœuds XOR
            ayant une relation parent/enfant
            Args:
                id1 (int) : id du nœud parent
                id2 (int) : id du nœud enfant
            """
            if self.graph[id1].get_label() != "^":
                raise ValueError(f"Le noeud d'id {id1} n'est pas un noeud XOR")
            if self.graph[id2].get_label() != "^":
                raise ValueError(f"Le noeud d'id {id2} n'est pas un noeud XOR")
            if id2 in self.graph[id1].get_children_ids():
                self.graph.fusion_2nodes(id1, id2, label="^")

        def associativite_COPIE(self, id: int) -> int:
            """
            Applique la règle d'associativité sur un nœud copie pourvu
            que son unique parent soit un nœud copie.
            Args:
                id (int): id du nœud copie
            Returns :
                0 si le graph est modifié 1 sinon
            """
            if self.graph[id].get_label() != "":
                raise ValueError(f"Le noeud d'id {id} n'est pas un noeud copie")
            if self.graph[self.graph[id].get_parent_ids()[0]].get_label() == "" and not self.graph[id].get_parent_ids()[
                                                                                            0] in self.graph.get_input_ids():
                self.graph.fusion_2nodes(self.graph[id].get_parent_ids()[0], id, label="")
                return 0
            return 1

        def involution_XOR(self, id1: int, id2: int) -> int:
            """
            Applique la règle d'involution entre un nœud XOR et un nœud copie pourvue que
            le nœud copie soit parent du nœud XOR
            Args:
                id1 (int): id du nœud parent copie
                id2 (int): id du nœud enfant XOR
            Returns :
                0 si le graph est modifié 1 sinon
            """
            id1, id2 = id2, id1
            if self.graph[id1].get_label() != "^":
                raise ValueError(f"Le noeud d'id {id1} n'est pas un noeud XOR")
            if self.graph[id2].get_label() != "":
                raise ValueError(f"Le noeud d'id {id2} n'est pas un noeud copie")
            if id2 in self.graph[id1].get_parent_ids():
                if self.graph[id1].get_parents()[id2] == 0 or self.graph[id1].get_parents()[id2] == 1:
                    return 1
                elif self.graph[id1].get_parents()[id2] % 2 == 0:
                    self.graph.remove_parallel_edges(id2, id1, rem_iso=False)
                else:
                    self.graph.remove_parallel_edges(id2, id1, rem_iso=False)
                    self.graph.add_edge(id2, id1)
                return 0

        def effacement(self, id: int) -> int:
            """
            Applique l'opération d'effacement entre un nœud copie et son unique parent pourvu que le
            nœud copie n'ait pas d'enfant
            Args:
                id (int) : id du nœud copie
            Returns :
                0 si le graph est modifié 1 sinon
            """
            if self.graph[id].get_label() != "":
                raise ValueError(f"Le noeud d'id {id} n'est pas un noeud copie")
            if len(self.graph[id].get_children_ids()) == 0 and not self.graph[id].get_parent_ids()[
                                                                       0] in self.graph.get_input_ids():
                parent_ids_backup = self.graph[self.graph[id].get_parent_ids()[0]].get_parent_ids()
                self.graph.remove_node_by_id(self.graph[id].get_parent_ids()[0], rem_iso=False)
                for i, parent_id in enumerate(parent_ids_backup):
                    if i == 0:
                        self.graph.add_edge(parent_id, id)
                    else:
                        self.graph.add_node(label="", parents={parent_id: 1}, children=None)
                return 0
            return 1

        def NON_through_XOR(self, id1: int, id2: int):
            """
            Applique l'opération NON à travers XOR pourvu que le nœud NON soit le parent du nœud XOR
            Args:
                id1 (int) : id du nœud parent NON
                id2 (int) : id du nœud enfant XOR
            """
            if self.graph[id1].get_label() != "~":
                raise ValueError(f"Le noeud d'id {id1} n'est pas un noeud NON")
            if self.graph[id2].get_label() != "^":
                raise ValueError(f"Le noeud d'id {id2} n'est pas un noeud XOR")
            if id1 in self.graph[id2].get_parent_ids():
                self.graph.add_edge(self.graph[id1].get_parent_ids()[0], id2)
                self.graph.remove_nodes_by_id(id1, rem_iso=False)
                enfant_id_XOR = self.graph[id2].get_children_ids()[0]
                self.graph.remove_edge(id2, enfant_id_XOR, rem_iso=False)
                self.graph.add_node("~", {id2: 1}, {enfant_id_XOR: 1})

        def NON_through_COPIE(self, id: int) -> int:
            """
            Applique l'opération NON à travers la copie pourvu que le nœud copie soit enfant du nœud NON
            Args:
                id (int) : id du nœud copie
            Returns :
                0 si le graph est modifié 1 sinon
            """
            # On vérifie que la méthode est appliquée avec en argument l'id d'un nœud copie

            if self.graph[id].get_label() != "":
                raise ValueError(f"Le noeud d'id {id} n'est pas un noeud copie")
            # L'unique parent du nœud copie est un nœud NON
            if self.graph[self.graph[id].get_parent_ids()[0]].get_label() == "~":
                # Seulement dans ce cas, on peut effectuer l'opération
                id_NON = self.graph[id].get_parent_ids()[0]
                parent_id_NON = self.graph[id_NON].get_parent_ids()[0]
                self.graph.add_edge(parent_id_NON, id)
                self.graph.remove_node_by_id(id_NON, rem_iso=False)
                children_map_listed = list(self.graph[id].get_children().items())
                for ch_id, multiplicity in children_map_listed:
                    self.graph.remove_parallel_edges(id, ch_id, rem_iso=False)
                    self.graph.add_node("~", {id: 1}, {ch_id: multiplicity})
                return 0
            return 1

        def involution_NON(self, id: int) -> int:
            """
            Applique l'opération involution de NON pourvu que l'unique parent du nœud NON d'id [id] soit
            un nœud NON
            Args:
                id(int) : l'id du nœud NON
            Returns :
                0 si le graph est modifié 1 sinon
            """
            # On vérifie que la méthode est appliquée avec en argument l'id d'un nœud NON
            if self.graph[id].get_label() != "~":
                raise ValueError(f"Le noeud d'id {id} n'est pas un noeud NON")
            # L'unique parent du nœud NON est un nœud NON
            if self.graph[self.graph[id].get_parent_ids()[0]].get_label() == "~" and not \
                    self.graph[id].get_parent_ids()[0] in self.graph.get_input_ids():
                # Seulement dans ce cas, on peut effectuer l'opération
                id_NON = self.graph[id].get_parent_ids()[0]
                # L'unique parent du nœud parent du nœud d'id [id]
                parent_id_NON = self.graph[id_NON].get_parent_ids()[0]
                # L'unique enfant du nœud d'id [id]
                child_id = self.graph[id].get_children_ids()[0]
                self.graph.add_edge(parent_id_NON, child_id)
                self.graph.remove_nodes_by_id(id, id_NON, rem_iso=False)
                return 0
            return 1

        def cofeuilles(self) -> list:
            """Trouve les cofeuilles du graphe qui ne sont pas directement reliés à une sortie"""
            L = []
            G = self.graph.copy()
            nodes = G.get_nodes()
            for node in nodes:
                id = node.get_id()
                if len(node.get_parent_ids()) == 0:
                    a = True
                    id_childrens = node.get_children_ids()
                    for id_children in id_childrens:
                        if id_children in G.outputs:
                            a = False
                    if a:
                        L.append(id)
            return L

        def evaluate(self):
            """Effectue les simplifications possibles au graphe jusqu'à ce qu'il n'y ait plus de cofeuilles"""
            G = self.graph
            L = self.cofeuilles()

            # Effectue les simplifications tant qu'il y a encore des co-feuilles
            while len(L) > 0:
                for id in L:
                    node = G.get_node_by_id(id)
                    id_childrens = node.get_children_ids()
                    for id_children in id_childrens:
                        if id_children in node.get_children_ids() and id_children in G.get_node_ids():
                            a = True
                            if G.get_node_by_id(id).get_label() == '^' or G.get_node_by_id(
                                    id).get_label() == '&' or G.get_node_by_id(id).get_label() == '|':
                                self.simpl_EN(id)
                                a = False
                            if a:
                                if (node.get_label() == "1" and G.get_node_by_id(id_children).get_label() == '') or (
                                        node.get_label() == "0" and G.get_node_by_id(id_children).get_label() == ''):
                                    self.simpl_COPY(id, id_children)
                                    a = False
                            if a:
                                if (node.get_label() == "1" and G.get_node_by_id(id_children).get_label() == '&') or (
                                        node.get_label() == "0" and G.get_node_by_id(id_children).get_label() == '&'):
                                    self.simpl_ET(id, id_children)
                                    a = False
                            if a:
                                if (a and node.get_label() == "1" and G.get_node_by_id(
                                        id_children).get_label() == '~') or (
                                        node.get_label() == "0" and G.get_node_by_id(id_children).get_label() == '~'):
                                    self.simpl_NON(id, id_children)
                                    a = False
                            if a:
                                if (a and node.get_label() == "1" and G.get_node_by_id(
                                        id_children).get_label() == '|') or (
                                        node.get_label() == "0" and G.get_node_by_id(id_children).get_label() == '|'):
                                    self.simpl_OU(id, id_children)
                                    a = False
                            if a:
                                if (a and node.get_label() == "1" and G.get_node_by_id(
                                        id_children).get_label() == '^') or (
                                        node.get_label() == "0" and G.get_node_by_id(id_children).get_label() == '^'):
                                    self.simpl_OUEX(id, id_children)
                                    a = False
                L = self.cofeuilles()

            # Les noeuds restants peuvent être des noeuds ET, OU, ou OUEX, donc il faut réappliquer la simplification des éléments neutres à tous les noeuds restants
            for id in G.get_node_ids():
                if G.get_node_by_id(id).get_label() == '^' or G.get_node_by_id(
                        id).get_label() == '&' or G.get_node_by_id(id).get_label() == '|':
                    self.simpl_EN(id)

        def evaluate2(self, verbose=False):
            """
            Effectue les simplifications possibles sur les cofeuilles et sur les opérateurs
            Args:
                verbose (bool) (default = False) : affiche intelligemment avec la commande display l'état du graph
            """

            G = self.graph
            ctrl = True
            nbTour = 0

            # Effectue les simplifications tant qu'il y a encore des simplifications possibles
            while (ctrl):

                ids = G.get_node_ids()
                ctrl = False
                L = self.cofeuilles()

                if verbose and (nbTour == 0 or nbTour % 2 == 1) and len(ids) > 8:
                    G.display()

                nbTour += 1

                for id in ids:
                    # On regarde si le nœud existe encore
                    if id in G.get_node_ids():

                        if id in L:
                            node = G.get_node_by_id(id)
                            id_childrens = node.get_children_ids()
                            for id_children in id_childrens:
                                if id_children in node.get_children_ids() and id_children in G.get_node_ids():
                                    a = True
                                    if G.get_node_by_id(id).get_label() == '^' or G.get_node_by_id(
                                            id).get_label() == '&' or G.get_node_by_id(id).get_label() == '|':
                                        self.simpl_EN(id)
                                        a = False
                                    if a:
                                        if (node.get_label() == "1" and G.get_node_by_id(
                                                id_children).get_label() == '') or (
                                                node.get_label() == "0" and G.get_node_by_id(
                                            id_children).get_label() == ''):
                                            self.simpl_COPY(id, id_children)
                                            a = False
                                    if a:
                                        if (node.get_label() == "1" and G.get_node_by_id(
                                                id_children).get_label() == '&') or (
                                                node.get_label() == "0" and G.get_node_by_id(
                                            id_children).get_label() == '&'):
                                            self.simpl_ET(id, id_children)
                                            a = False
                                    if a:
                                        if (a and node.get_label() == "1" and G.get_node_by_id(
                                                id_children).get_label() == '~') or (
                                                node.get_label() == "0" and G.get_node_by_id(
                                            id_children).get_label() == '~'):
                                            self.simpl_NON(id, id_children)
                                            a = False
                                    if a:
                                        if (a and node.get_label() == "1" and G.get_node_by_id(
                                                id_children).get_label() == '|') or (
                                                node.get_label() == "0" and G.get_node_by_id(
                                            id_children).get_label() == '|'):
                                            self.simpl_OU(id, id_children)
                                            a = False
                                    if a:
                                        if (a and node.get_label() == "1" and G.get_node_by_id(
                                                id_children).get_label() == '^') or (
                                                node.get_label() == "0" and G.get_node_by_id(
                                            id_children).get_label() == '^'):
                                            self.simpl_OUEX(id, id_children)
                                            a = False
                                    ctrl = not a
                            continue

                        # Les simplifications suivantes ne sont pas réalisables sur les inputs et les outputs
                        if id in G.get_input_ids() + G.get_output_ids():
                            continue

                        if G[id].get_label() == "^":
                            children_ids = G[id].get_children_ids()
                            for child_id in children_ids:
                                if G[child_id].get_label() == "^":
                                    self.associativite_XOR(id, child_id)
                                    ctrl = True
                                    continue
                        elif G[id].get_label() == "":
                            if self.associativite_COPIE(id) == 0:
                                ctrl = True
                                continue
                            if self.effacement(id) == 0:
                                ctrl = True
                                continue
                            if self.NON_through_COPIE(id) == 0:
                                ctrl = True
                                continue
                            children_ids = G[id].get_children_ids()
                            for child_id in children_ids:
                                if G[child_id].get_label() == "^":
                                    if self.involution_XOR(id, child_id) == 0:
                                        ctrl = True
                                        break
                        elif G[id].get_label() == "~":
                            if self.involution_NON(id) == 0:
                                ctrl = True
                                continue
                            children_ids = G[id].get_children_ids()
                            for child_id in children_ids:
                                if G[child_id].get_label() == "^":
                                    self.NON_through_XOR(id, child_id)
                                    ctrl = True
                                    break

            # Les noeuds restants peuvent être des noeuds ET, OU, ou OUEX, donc il faut réappliquer la simplification des éléments neutres à tous les noeuds restants
            for id in G.get_node_ids():
                if G.get_node_by_id(id).get_label() == '^' or G.get_node_by_id(
                        id).get_label() == '&' or G.get_node_by_id(id).get_label() == '|':
                    self.simpl_EN(id)

            if verbose:
                G.display()

        def from_list_to_string(L: list) -> str:
            """Prend en argument une liste L composée dans les cas qui nous interesseront d'int ou de str et renvoie la
            chaine de caractères obtenus par la somme des caractères composant le tableau"""
            s = ""
            for i in range(len(L)):
                if not isinstance(L[i], str):
                    s += str(L[i])
                else:
                    s += L[i]
            return s

        @classmethod
        def encodeur(cls):  # -> open_digraph.bool_circ
            """Génère un encodeur"""
            G = open_digraph.bool_circ.parse_parentheses("^(x1)(x2)(x4)", "^(x1)(x3)(x4)", "^(x2)(x3)(x4)")[0]
            for id in [22, 20, 15, 17]:
                G.add_output_node(id)
            # On impose l'ordre des inputs et des outputs
            G.set_input_ids([23, 16, 18, 21])
            G.set_output_ids([11, 6, 24, 1, 26, 27, 25])
            return open_digraph.bool_circ(G)

        @classmethod
        def decodeur(cls):  # -> open_digraph.bool_circ
            """Génère un décodeur"""
            g1 = open_digraph.bool_circ.parse_parentheses("^(x1)(x2)(x4)", "^(x1)(x3)(x4)", "^(x2)(x3)(x4)")[0]
            g2 = open_digraph.bool_circ.parse_parentheses("^(&(x1)(~(x3))(x2))",
                                                          "^(&(x1)(~(x2))(x3))",
                                                          "^(&(x2)(~(x1))(x3))",
                                                          "^(&(x1)(x3)(x2))")[0]
            G = g2.compose2(g1)
            for id in [0, 10, 5]:
                G.add_input_node(id)
            G.add_edges([(22, 30), (15, 44), (17, 37), (20, 24)])
            # On impose l'ordre des inputs et des outputs
            G.set_input_ids([64, 65, 23, 63, 16, 18, 21])
            G.set_output_ids([31, 45, 38, 25])
            return open_digraph.bool_circ(G)

        def profondeur(self, n: int, noeuds_input: list, noeuds_output: list):
            """Fonction qui calcule la profondeur d'un graphe"""

            def bis(noeud, memo, noeuds_output) -> int:
                """Fonction auxiliaire pour calculer la profondeur d'un graphe"""
                if noeud in memo:
                    return memo[noeud]

                if noeud in noeuds_output:
                    memo[noeud] = 0
                    return 0

                profondeur_max = -1

                for enfant in self.graph.get_node_by_id(noeud).get_children():
                    profondeur_enfant = bis(enfant, memo, noeuds_output)
                    profondeur_max = max(profondeur_max, profondeur_enfant)

                memo[noeud] = profondeur_max + 1
                return profondeur_max + 1

            memo = {}
            profondeur_max = 0

            for noeud_input in noeuds_input:
                profondeur_max_input = bis(noeud_input, memo, noeuds_output)
                profondeur_max = max(profondeur_max, profondeur_max_input)

            print(f"La profondeur du graphe half_adder({n}) est :", profondeur_max - 1)

        @classmethod
        def calcul(cls, n: int, a: int, etape_int=False):
            """Fonction de test pour les fonctions evaluate() et adder()

            Prend en argument un entier n représentant la taille des registres et [a] le nombre décimal représentant
            la somme que nous voulons effectuer, et renvoie une chaine de caractères représentant le résultat en
            binaire ex : Nous voulons effectuer l'addition de 11 et 7 avec une retenue sur des registres de taille
            2**2. On écrit 11 et 7 en binaire sur 2**2 bits avec la retenue : 11 = 1011, 7 = 0111, 1 = 1. Le nombre
            binaire obtenu est 101101111, ce qui vaut 367 en décimal. De plus, on sait que 11+7+1=19, qui vaut en
            décimal 10011. Donc l'appel calcul(2, 367) effectue l'addition de 11 et 7 avec une retenue, et écrit le
            résultat en binaire, soit normalement 10011"""
            A = open_digraph.bool_circ.adder(n)
            A1 = open_digraph.bool_circ.from_int_to_registre(a, 2 ** (n + 1) + 1)
            H = A.graph.compose2(A1.graph)
            V = open_digraph.bool_circ(H)

            V.evaluate2(verbose=etape_int)

            R = []
            for out in V.graph.get_output_ids():
                R.append(V.graph.get_node_by_id(V.graph.get_node_by_id(out).get_parent_ids()[0]).get_label())

            s = bin(a)[2:]
            if len(s) > 2 ** (n + 1) + 1:
                raise ValueError("Taille du registre trop faible")
            else:
                L = []
                diff = -1
                for i in range(len(s)):
                    L.append(int(s[i]))
                if 2 ** (n + 1) + 1 - len(s) != 0:
                    diff += 2 ** (n + 1) + 1 - len(s)
                T = [0 for i in range(diff + 1)]
                L = T + L

            print(
                f"L'addition de {open_digraph.bool_circ.from_list_to_string(L[0:2 ** n])} et {open_digraph.bool_circ.from_list_to_string(L[2 ** n:2 ** (n + 1)])}, avec une retenue de {L[2 ** (n + 1)]} fait : {open_digraph.bool_circ.from_list_to_string(R)}")

        @classmethod
        def verificationHamming(cls, vals: list, allCases=0, allNumbers=False, showingMode=False):
            """
            Permet de vérifier la propriété de Hamming dans le cas d'un encodeur et d'un décodeur de taille 4 pour les
            valeurs présentes dans [vals]. Ces valeurs doivent pouvoir s'écrire sur 4bits en base 2
            Args :
                vals (<int>list) : La liste des valeurs en base 10 pour lesquelles on souhaite tester la propriété de
                  Hamming
                allCases (int) (default = 0) : Spécifie le nombre de tests introduisant deux erreurs entre l'encodeur et
                  le décodeur. Si [allCases] = 0, l'algorithme teste toutes les couples de positions possibles. Si
                  [allCase] != 0, l'algorithme fait [allCases] tests en générant pour chaque test, deux positions
                  aléatoires d'erreur. Les couples de positions sont deux à deux différents pour chaque test.
                allNumbers (bool) (default = False) : Si True, l'échantillon testé sera l'ensemble des nombres pouvant
                  s'écrire en base 2 sur 4 bits, sinon l'échantillon sera [vals]
                showingMode (bool) (default = False) : Si True, présente un exemple en appelant evaluate2 avec
                  l'argument verbose
            Raises (ValueError) : Si une valeur dans [val] ne peut pas s'écrire sur 4 bits une fois convertie en base 2
            Raises (ValueError) : Si [allCases]>42 ce qui correspond à toutes les possibilités possibles
            """

            def introduitErreur(pos: list) -> open_digraph:
                """
                Génère un open_digraph composé de 8 branches en parallèle sur lesquelles se trouve une erreur
                aux positions indiquée par [pos]. La position 0 représentant la branche la plus à gauche et la position
                6 à droite
                Args:
                    pos (<int>list): La liste des positions des erreurs
                Returns : L'open_digraph émulant le bruit entre l'encodeur et le décodeur
                Raises (ValueError) : Si une position dans [pos] n'est pas comprise entre 0 et 6.
                """
                for x in pos:
                    if x < 0 or x >= 7:
                        raise ValueError("Une position dans la liste [pos] n'est pas comprise entre 0 et 6 (inclus)")
                G = open_digraph.empty()
                for k in range(7):
                    i = node(0, "", {}, {1: 1})
                    n = node(1, "", {0: 1}, {2: 1})
                    o = node(2, "", {1: 1}, {})
                    if (6 - k) in pos:
                        n.set_label("~")
                    G.iparallel(open_digraph([0], [2], [i, n, o]))
                return G

            if allCases > 42 or allCases < 0:
                raise ValueError("[allCases] doit être compris entre 0 et 41")

            if allNumbers:
                vals = [i for i in range(16)]

            # On présente un exemple, avec un input aléatoire entre 0 et 15, puis la composition d'un encodeur
            # et d'un décodeur de Hamming
            if showingMode:
                R = []
                x = random.randint(0, 15)
                A = open_digraph.bool_circ.from_int_to_registre(x, 2 ** 3 - 4)
                G0 = open_digraph.bool_circ.encodeur()
                G1 = open_digraph.bool_circ.decodeur()
                G0.graph.icompose2(A.graph)
                G1.graph.icompose2(G0.graph)
                G1.evaluate2(verbose=True)
                for out in G1.graph.get_output_ids():
                    R.append(G1.graph[G1.graph[out].get_parent_ids()[0]].get_label())
                res_input = bin(3)[2:]
                res_output = open_digraph.bool_circ.from_list_to_string(R)
                errorB10 = abs(int(res_input, 2) - int(res_output, 2))
                print(f"ENTRÉE = {res_input:0>4} | SORTIE = {res_output} | ERREUR = {bin(errorB10)[2:]:0>4}")
                return

            print("VERIFICATION DE LA PROPRIÉTÉ PRINCIPALE DU CODE HAMMING :")
            print("---------------------------------------------------------")
            print("SANS INTRODUIRE D'ERREUR ENTRE L'ENCODEUR ET LE DÉCODEUR :")
            for i in vals:
                R = []
                A = open_digraph.bool_circ.from_int_to_registre(i, 2 ** 3 - 4)
                G0 = open_digraph.bool_circ.encodeur()
                G1 = open_digraph.bool_circ.decodeur()
                G0.graph.icompose(A.graph)
                G1.graph.icompose(G0.graph)
                G1.evaluate2()
                for out in G1.graph.get_output_ids():
                    R.append(G1.graph[G1.graph[out].get_parent_ids()[0]].get_label())
                res_input = bin(i)[2:]
                res_output = open_digraph.bool_circ.from_list_to_string(R)
                errorB10 = abs(int(res_input, 2) - int(res_output, 2))
                print(f"ENTRÉE = {res_input:0>4} | SORTIE = {res_output} | ERREUR = {bin(errorB10)[2:]:0>4}")
            print("---------------------------------------------------------")
            print("EN INTRODUISANT UNE ERREUR ENTRE L'ENCODEUR ET LE DÉCODEUR :")
            for i in vals:
                for k in range(7):
                    R = []
                    A = open_digraph.bool_circ.from_int_to_registre(i, 2 ** 3 - 4)
                    G0 = open_digraph.bool_circ.encodeur()
                    G1 = introduitErreur([k])
                    G2 = open_digraph.bool_circ.decodeur()
                    G0.graph.icompose(A.graph)
                    G1.icompose(G0.graph)
                    G2.graph.icompose(G1)
                    G2.evaluate2()
                    for out in G2.graph.get_output_ids():
                        R.append(G2.graph[G2.graph[out].get_parent_ids()[0]].get_label())
                    res_input = bin(i)[2:]
                    res_output = open_digraph.bool_circ.from_list_to_string(R)
                    errorB10 = abs(int(res_input, 2) - int(res_output, 2))
                    print(f"ENTRÉE = {res_input:0>4} | SORTIE = {res_output} | ERREUR = {bin(errorB10)[2:]:0>4} | "
                          f"BRUIT sur 1 bit en position {k}")
            print("---------------------------------------------------------")
            print("EN INTRODUISANT PLUSIEURS ERREURS ENTRE L'ENCODEUR ET LE DÉCODEUR :")

            # Génération des positions d'erreurs

            if allCases == 0 or allCases == 42:
                cases = [(i, j) for i in range(7) for j in range(7) if i != j]

            else:
                ens = [(i, j) for i in range(7) for j in range(7) if i != j]
                cases = []
                for i in range(allCases):
                    case = random.choice(ens)
                    ens.remove(case)
                    cases.append(case)

            for i in vals:
                for k1, k2 in cases:
                    if k1 != k2:
                        R = []
                        A = open_digraph.bool_circ.from_int_to_registre(i, 2 ** 3 - 4)
                        G0 = open_digraph.bool_circ.encodeur()
                        G1 = introduitErreur([1, 2])
                        G2 = open_digraph.bool_circ.decodeur()
                        G0.graph.icompose(A.graph)
                        G1.icompose(G0.graph)
                        G2.graph.icompose(G1)
                        G2.evaluate2()
                        for out in G2.graph.get_output_ids():
                            R.append(G2.graph[G2.graph[out].get_parent_ids()[0]].get_label())
                        res_input = bin(i)[2:]
                        res_output = open_digraph.bool_circ.from_list_to_string(R)
                        errorB10 = abs(int(res_input, 2) - int(res_output, 2))
                        print(
                            f"ENTRÉE = {res_input:0>4} | SORTIE = {res_output} | ERREUR = {bin(errorB10)[2:]:0>4} | "
                            f"BRUIT sur 2 bit en position ({k1},{k2})")