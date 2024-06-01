from modules.mixins.node.get_set_node import Get_Set_Node

class Add_Remove(Get_Set_Node):
    """Ajout ou suppression de parents ou d'enfants pour un noeud"""
    def add_parent_id(self, parent_id: int):
        if parent_id in self.parents:
            self.parents[parent_id] += 1
        else:
            self.parents[parent_id] = 1

    def add_child_id(self, child_id: int):
        if child_id in self.children:
            self.children[child_id] += 1
        else:
            self.children[child_id] = 1

    def remove_parent_once(self, parent_id: int):
        if parent_id in self.parents:
            if self.parents[parent_id] > 1:
                self.parents[parent_id] -= 1
            else:
                del self.parents[parent_id]

    def remove_child_once(self, child_id: int):
        if child_id in self.children:
            if self.children[child_id] > 1:
                self.children[child_id] -= 1
            else:
                del self.children[child_id]

    def remove_parent_id(self, parent_id: int):
        del self.parents[parent_id]

    def remove_child_id(self, child_id: int):
        del self.children[child_id]