class Get_Set_Node:
    """Ensemble des getters et des setters utiles à la classe node"""
    # Getter
    def get_id(self) -> int:
        return self.id

    def get_label(self) -> str:
        return self.label

    def get_parents(self) -> dict:
        return self.parents

    def get_children(self) -> dict:
        return self.children

    def get_parent_ids(self) -> list:
        return list(self.parents.keys())
    
    def get_children_ids(self) -> list:
        return list(self.children.keys())
    
    # Setter
    def set_id(self, id: int):
        self.id = id

    def set_label(self, label: str):
        self.label = label

    def set_parents(self, parents):
        self.parents = parents

    def set_children(self, children):
        self.children = children

    def set_parent_ids(self, parents_ids: dict):
        self.parents = parents_ids

    def set_children_ids(self, children_ids: dict):
        self.children = children_ids