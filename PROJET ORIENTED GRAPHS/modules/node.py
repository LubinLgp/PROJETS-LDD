from modules.mixins.node.add_remove_node import Add_Remove
from modules.mixins.node.degree import Degree

class node(Add_Remove, Degree):
    def __init__(self, identity, label, parents, children):
        '''
        identity: int; its unique id in the graph
        label: string;
        parents: int->int dict; maps a parent node's id to its multiplicity
        children: int->int dict; maps a child node's id to its multiplicity
        '''
        self.id = identity
        self.label = label
        self.parents = parents
        self.children = children

    # Native methods
    def __str__(self) -> str:
        return self.label

    def __repr__(self) -> str:
        s = (f"{self.label}: parents:[")
        for nodep in self.parents:
            s += f"{nodep},"
        s += "], childrens:["
        for nodec in self.children:
            s += f"{nodec},"
        s += "]"
        return s

    # Methods
    def copy(self):  # -> node
        """ Copie le noeud [self] et tous ses arguments, ainsi l'instance copiée et le noeud
        original évoluent indépendamment l'un de l'autre
        Args :
            self
        Return:
            node : le noeud copié
        """
        return node(self.id, self.label, self.parents.copy(), self.children.copy())