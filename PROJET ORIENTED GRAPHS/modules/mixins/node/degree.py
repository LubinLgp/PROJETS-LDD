from modules.mixins.node.get_set_node import Get_Set_Node

class Degree(Get_Set_Node):
    # Methods for boolean circuits
    def indegree(self):
        """ Renvoie le degrÃ© entrant du noeud [self]"""
        res = 0
        for multiplicity in self.get_parents().values():
            res += multiplicity
        return res


    def outdegree(self):
        """ Renvoie le degrÃ© sortant du noeud [self]"""
        res = 0
        for multiplicity in self.get_children().values():
            res += multiplicity
        return res


    def degree(self):
        """ Renvoie le degrÃ© du noeud [self]"""
        return self.indegree() + self.outdegree()
