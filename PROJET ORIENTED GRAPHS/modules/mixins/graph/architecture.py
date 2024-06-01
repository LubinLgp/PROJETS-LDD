from modules.open_digraph import node
from modules.matrix import *
import random

class Architecture:
    # Getter
    def get_input_ids(self) -> list:
        return self.inputs

    def get_output_ids(self) -> list:
        return self.outputs

    def get_id_node_map(self) -> dict:
        return self.nodes

    def get_next_id(self) -> int:
        return self.next_id

    def get_nodes(self) -> list:  # <node> list
        return list(self.nodes.values())

    def get_node_ids(self) -> list:  # <int> list
        return list(self.nodes.keys())

    def get_node_by_id(self, ide: int) -> node:
        return self.nodes[ide]

    def get_nodes_by_ids(self, ids: list) -> list:  # node list
        return list(self.get_node_by_id(id) for id in ids)

    # Setter
    def set_inputs(self, inputs):
        self.inputs = inputs

    def set_outputs(self, outputs):
        self.outputs = outputs

    def set_input_ids(self, input_ids: list):
        self.inputs = input_ids

    def set_output_ids(self, output_ids: list):
        self.outputs = output_ids

    def set_nodes(self, nodes_map: dict):
        self.nodes = nodes_map

    def add_input_id(self, id: int):
        self.inputs.append(id)

    def add_output_id(self, id: int):
        self.outputs.append(id)


    def add_edge(self, src: int, tgt: int):
        parent = self.get_id_node_map()[src]
        children = self.get_id_node_map()[tgt]
        parent.add_child_id(tgt)
        children.add_parent_id(src)

    def add_edges(self, edges: list):
        for i in range(len(edges)):
            src = edges[i][0]
            tgt = edges[i][1]
            self.add_edge(src, tgt)


    def add_node(self, label='', parents=None, children=None) -> int:
        id = self.new_id()
        if (parents == None):
            parents = {}
        if (children == None):
            children = {}
        n = node(id, label, parents, children)
        if not id in self.nodes:
            self.nodes[id] = n
        for tgt, multi in children.items():
            self.nodes[tgt].parents[id] = multi
        for src, multi in parents.items():
            self.nodes[src].children[id] = multi
        return id

    @classmethod
    def empty(cls):  # cls est un allias de open_digraph
        return cls([], [], {})

    def remove_isolated(self, id: int):
        """
            Inputs: (int) id -> the id of the node we're intrested in.
            Output: removes the node whose id is [id] if it is
            isolated
        """
        if id in self.get_node_ids():
            n = self.get_node_by_id(id)
            if not (n.get_children()) and not (n.get_parents()):
                self.nodes.pop(id, None)
                if id in self.get_input_ids():
                    self.inputs.remove(id)
                elif id in self.get_output_ids():
                    self.outputs.remove(id)

    def remove_edge(self, src: int, trg: int, rem_iso=True):
        l = self.get_node_ids()
        if src in l and trg in l:
            n = self.get_node_by_id(src)
            if trg in n.get_children_ids():
                self.nodes[src].remove_child_once(trg)
                self.nodes[trg].remove_parent_once(src)
            if rem_iso:
                self.remove_isolated(src)
                self.remove_isolated(trg)

    def remove_parallel_edges(self, src: int, tgt: int, rem_iso=True):
        self.nodes[src].remove_child_id(tgt)
        self.nodes[tgt].remove_parent_id(src)


    def remove_node_by_id(self, id: int, rem_iso=True):
        l = self.get_node_ids()
        if id in l:
            n = self.get_node_by_id(id)
            for c in n.get_children_ids():
                self.remove_parallel_edges(id, c,rem_iso)
            for p in n.get_parent_ids():
                self.remove_parallel_edges(p, id,rem_iso)
            self.nodes.pop(id, None)
            if id in self.get_input_ids():
                self.inputs.remove(id)
            elif id in self.get_output_ids():
                self.outputs.remove(id)

    def remove_edges(self, *args, rem_iso=True):
        for src, tgt in args:
            self.remove_edge(src, tgt)


    def remove_several_parallel_edges(self, *args, rem_iso=True):
        for src, tgt in args:
            self.remove_parallel_edges(src, tgt)


    def remove_nodes_by_id(self, *args, rem_iso=True):
        for node_id in args:
            self.remove_node_by_id(node_id)


    def add_input_node(self, id: int, label=''):
        id_noeuds = self.get_node_ids()
        inp = self.get_input_ids()
        out = self.get_output_ids()
        if (id in id_noeuds) and (not id in inp) and (not id in out):
            new_id = self.add_node(label, {}, {id: 1})
            self.inputs.append(new_id)
        return new_id

    def add_output_node(self, id: int, label='') -> int:
        id_noeuds = self.get_node_ids()
        inp = self.get_input_ids()
        out = self.get_output_ids()
        if (id in id_noeuds) and (not id in inp) and (not id in out):
            new_id = self.add_node(label, {id: 1}, {})
            self.outputs.append(new_id)
        return new_id

    def init_next_id(self) -> int:
        """ Initialise l'attribut : open_digraph.next_id"""
        try:
            # la liste est non vide i.e le graphe possède déja un nœud
            return max(self.get_id_node_map().keys()) + 1
        except:
            return 0

    def increment_next_id(self):
        """ Incrémente l'attribut : open_digraph.next_id"""
        self.next_id += 1

    def new_id(self) -> int:
        """ Retourne l'id du prochain noeud ajouté dans le graph"""
        res = self.get_next_id()
        self.increment_next_id()
        return res

    def min_id(self) -> int:
        if not self.nodes:
            return None
        return min(self.nodes.keys())


    def max_id(self) -> int:
        if not self.nodes:
            return None
        return max(self.nodes.keys())


    def shift_indices(self, n: int):
        """Ajoute à l'indice de tous les noeuds de [self], l'entier [n]
        Args:
            n (int): rang du décalage
        """

        def add_list(l: list, x: int) -> list:
            """ Modifie la liste [l] et ajoute à chaque element l'entier [x]
            Args:
                l (<int> list): la liste que nous allons modifier
                x (int): entier a additionner (peut être négatif)
            Returns:
                list: La liste modifiée
            """
            n = len(l)
            for i in range(n):
                l[i] += x
            return l

        def add_key_map(d: dict, x: int) -> dict:
            """ Renvoie le dictionnaire formé à partir de [d] auquel on a ajouté l'entier
            [x] à chaque clé sans modifier la valeur associé à la clé
            Args:
                d (<int : ... > dict): le dictionnaire source
                x (int): entier à additionner (peut être négatif)
            Returns:
                dict: Le nouveau dictionnaire
            """
            res = dict()
            keys = d.keys()
            for u in keys:
                value = d[u]
                res.update({u + n: value})
            return res

        self.set_input_ids(add_list(self.get_input_ids(), n))
        self.set_output_ids(add_list(self.get_output_ids(), n))
        self.set_nodes(add_key_map(self.get_id_node_map(), n))
        nodes = self.get_nodes()
        for node in nodes:
            node.set_id(node.get_id() + n)
            parent_ids, parent_multiplicities = list(node.get_parents().keys()), node.get_parents().values()
            child_ids, child_multiplicities = list(node.get_children().keys()), node.get_children().values()
            node.set_parent_ids(dict(zip(add_list(parent_ids, n), parent_multiplicities)))
            node.set_children_ids(dict(zip(add_list(child_ids, n), child_multiplicities)))

    def copy(self):
        nodes_iter = [node.copy() for node in self.nodes.values()]
        return self.__class__(self.inputs.copy(), self.outputs.copy(), nodes_iter)

    def is_well_formed(self) -> bool:
        inp = self.get_input_ids()
        out = self.get_output_ids()
        noeuds = self.get_id_node_map()
        id_noeuds = self.get_node_ids()
        for i in inp:
            if not i in id_noeuds:
                return False
            else:
                n_i = self.get_node_by_id(i)
                fils = n_i.get_children()
                parents = n_i.get_parents()
                if len(parents) != 0 or len(fils) != 1 or fils[n_i.get_children_ids()[0]] != 1:
                    return False
        for o in out:
            if not o in id_noeuds:
                return False
            else:
                n_o = self.get_node_by_id(o)
                fils = n_o.get_children()
                parents = n_o.get_parents()
                if len(fils) != 0 or len(parents) != 1 or parents[n_o.get_parent_ids()[0]] != 1:
                    return False
        for clé in id_noeuds:
            if not isinstance(noeuds[clé], node) or noeuds[clé].get_id() != clé:
                return False
            else:
                nod = noeuds[clé]
                enfants = nod.get_children()
                if len(enfants) != 0:
                    for id in list(enfants.keys()):
                        mul = enfants[id]
                        enfant_i = self.get_node_by_id(id)
                        parents_de_enfant_i = enfant_i.get_parents()
                        if not clé in list(parents_de_enfant_i.keys()):
                            return False
                        elif parents_de_enfant_i[clé] != mul:
                            return False
        return True

    def assert_is_well_formed(self):
        if not(self.is_well_formed()):
            raise Exception("graph not well formed")


    def fusion_2nodes(self, id1, id2, label=None):
        """_summary_
        Args:
            id1 & id2 (int): ids des 2 noeuds que nous allons fusionner
            label (str): label du nouveau noeud. Si laissé à [None] le label est vide
        Return:
            l'id du noeud formé par la fusion
        """
        if id1 in self.get_node_by_id(id2).get_parent_ids() or id1 in self.get_node_by_id(id2).get_children_ids() :
            self.remove_parallel_edges(id1, id2,rem_iso=False)

        Parents = self.get_node_by_id(id1).get_parents().copy()
        P2 = self.get_node_by_id(id2).get_parents().copy()
        for k in P2.keys():
            if k in Parents.keys():
                Parents[k] += P2[k]
            else:
                Parents[k] = P2[k]

        Children = self.get_node_by_id(id1).get_children().copy()
        C2 = self.get_node_by_id(id2).get_children().copy()
        for k in C2.keys():
            if k in Children.keys():
                Children[k] += C2[k]
            else:
                Children[k] = C2[k]

        self.remove_node_by_id(id1, rem_iso=False)
        self.remove_node_by_id(id2, rem_iso=False)

        if label == None :
            res = self.add_node(self.get_node_by_id(id1).get_label(),parents=Parents,children=Children)
        else :
            res = self.add_node(label = label,parents=Parents,children=Children)

        return res

    def fusion_nodes(self, L : list, label=None):
        """_summary_
        Args:
            L (list <int>): liste des ids des noeuds que nous allons fusionner
            label (str): label du nouveau noeud. Si laissé à [None] le label est vide
        Returns:
            L'id du noeud formé par les fusions
        """
        n = len(L)
        res = self.fusion_2nodes(L[0], L[1],label=label)
        for i in range(2,n):
            res = self.fusion_2nodes(res, L[i], label=label)
        return res