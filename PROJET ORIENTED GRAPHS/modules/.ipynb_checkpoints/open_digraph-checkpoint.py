class node :
    def __init__(self,identity, label, parents, children):
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

    # Getter
    def get_id(self) -> int :
        return self.id

    def get_label(self) -> str :
        return self.label

    def get_parents(self) -> dict :
        return self.parents

    def get_children(self) -> dict :
        return self.children

    def get_parent_ids(self) -> list :
        return list(self.parents.keys())

    def get_children_ids(self) -> list :
        return list(self.children.keys())

    # Setter
    def set_id(self,id) :
        self.id = id

    def set_label(self,label) :
        self.label = label

    def set_parent_ids(self,parents_ids) :
         # parents_ids =  dict
         self.parents = parents_ids

    def set_children_ids(self,children_ids):
        # children_ids = dict
        self.children = children_ids


    # Native methods

    def __str__(self) -> str:
        return "Node(\n id = {} \n label = {} \n parents = {} \n children = {} )".format(self.id,self.label,self.parents,self.children)

    def __repr__(self) -> str:
        return self.__str__() + "\n --------------- \n"

    # Methods

    def copy(self) :
        return node(self.id,self.label,self.parents.copy(),self.children.copy())

    def add_parent_id(self,id):
        if not id in self.parents :
            self.parents[id] = 1
        else :
            self.parents[id] += 1

    def add_child_id(self,id):
        if not id in self.children :
            self.children[id] = 1
        else :
            self.children[id] += 1

    def remove_parent_once(self, id):
        """ removes an occurency (an instance or a multiplicity) of the parent
        whose 'id' is passed as a parameter """
        try:
            if (self.get_parents()[id] > 1):
                self.parents[id] -= 1
            else:
                self.parents.pop(id, None)
        except KeyError:
            print("Node whose id = {} has no parent whose id = {}\n --------------- \n".format(self.get_id(), id))

    def remove_child_once(self, id):
        """ removes an occurency (an instance or a multiplicity) of the child
        whose 'id' is passed as a parameter """
        try:
            if (self.get_children()[id] > 1):
                self.children[id] -= 1
            else:
                self.children.pop(id, None)
        except KeyError:
            print("Node whose id = {} has no child whose id = {}\n --------------- \n".format(self.get_id(), id))

    def remove_parent_id(self, id):
        """ removes from the parents' dict a parent
        whose 'id' is passed as a parameter """
        try:
            self.parents.pop(id, None)
        except KeyError:
            print("Node whose id = {} has no parent whose id = {}\n --------------- \n".format(self.get_id(), id))


    def remove_child_id(self, id):
        """ removes from the children's dict a child whose 'id'
        is passed as a parameter """
        try:
            self.children.pop(id, None)
        except KeyError:
            print("Node whose id = {} has no child whose id = {}\n --------------- \n".format(self.get_id(), id))


class open_digraph: # for open directed graph
    def __init__(self, inputs, outputs, nodes):
        '''
        inputs: int list; the ids of the input nodes
        outputs: int list; the ids of the output nodes
        nodes: node iter;
        '''
        self.inputs = inputs
        self.outputs = outputs
        self.nodes = {node.id:node for node in nodes} # self.nodes: <int,node> dict
        self.next_id = max(self.nodes.keys())+1

    @classmethod
    def empty(cls): #cls est un allias de open_digraph
        return cls([],[],{})

    # Getter
    def get_input_ids(self) -> list :
        return self.inputs

    def get_output_ids(self) -> list :
        return self.outputs

    def get_id_node_map(self) -> dict :
        return self.nodes

    def get_next_id(self) -> int :
        return self.next_id

    def get_nodes(self) -> list :
        return list(self.nodes.values())

    def get_node_ids(self) -> list :
        return list(self.nodes.keys())

    def get_node_by_id(self,id) -> node :
        if id in self.nodes.keys():
            return self.nodes[id]
        else:
            raise AttributeError

    def get_nodes_by_ids(self,ids) -> list :
        return list(self.get_node_by_id(id) for id in ids)


    # Setter
    def set_input_ids(self,input_ids):
        self.inputs = input_ids

    def set_output_ids(self,output_ids):
        self.inputs = output_ids

    # Native methods

    def __str__(self) -> str:
        return "Digraph(\n input = {} \n output = {} \n nodes = {} )".format(self.inputs,self.outputs,self.nodes)

    def __repr__(self) -> str:
        return self.__str__()

    def __getitem__(self,id) :
        return self.get_node_by_id(self,id)



    # Methods

    def copy(self) :
        nodes_iter = [node.copy() for node in self.nodes.values()]
        return open_digraph(self.inputs.copy(),self.outputs.copy(),nodes_iter)

    def increment_next_id(self):
        self.next_id += 1


    def add_input_id(self,id):
        self.inputs.append(id)

    def add_output_id(self,id):
        self.outputs.append(id)

    def new_id(self) -> int:
        res = self.get_next_id()
        self.increment_next_id()
        return res

    def add_edge(self, src, tgt):
        parent = self.get_id_node_map()[src]
        children = self.get_id_node_map()[tgt]
        parent.add_child_id(tgt)
        children.add_parent_id(src)

    def add_edges(self,edges):
        for src,tgt in edges :
            self.add_edge(src,tgt)

    def add_node(self,label='',parents=None, children=None) -> int:
        # parents , children = dict
        id = self.new_id()
        if(parents == None) :
            parents = {}
        if(children == None) :
            children = {}
        n = node(id,label,parents,children)
        if not id in self.nodes :
            self.nodes[id] = n
        else :
            raise ValueError("id déja présent dans di_graph.nodes")
        #Pour chaque enfant de [n], ce dernier est son parent
        for tgt, multi in children.items() :
            self.nodes[tgt].parents[id] = multi
        #Pour chaque parent de [n], ce dernier est son enfant
        for src , multi in parents.items() :
            self.nodes[src].children[id] = multi
        return id

    def remove_edge(self, src, trg):
        """ removes an edge between the nodes whose ids are respectively 'src' and 'trg' """
        if src in self.get_node_ids():
		n = self.get_node_by_id(src)
		if trg in n.get_children_ids():
			self.nodes[src].remove_child(trg) # src is a parent of trg
			self.nodes[trg].remove_parent(src) #trg is a child of src
		else:
			raise KeyError("Node {} has no child {}".format(src, trg))
        else:
            raise KeyError("There's no node or input whose 'id' is = {}".format(src))

    def remove_parallel_edges(self, src, trg):
        """ removes all the directed edges linking the source node (whose 'id' is 'src') and
        the target node (whose 'id' is 'trg') """
        if src in self.get_node_ids():
            if trg in self.get_node_ids():
                self.nodes[trg].remove_parent_id(src) # src is a parent for trg
                self.nodes[src].remove_child_id(trg) # trg is a child for src
            elif trg in self.get_output_ids():
                self.output[trg].remove_parent_id(src) # src is a parent for trg
                self.nodes[src].remove_child_id(trg)  # trg is a child for src
            else:
                raise KeyError("Edge between [node, output] or [node, node] but there's no node or output whose 'id' = {}".format(trg))
        else:
            raise KeyError("There's no node or input whose 'id' is = {}".format(src))



    def remove_node_by_id(self, id):
	    """ Removes a node from the graph, with all the corresponding
	    edges """
	    if id in self.get_node_ids():
		    # The node whose id is 'id' exists
		    n = self.get_node_by_id(id)
		    for k in n.get_children_ids():
		    # remove outward edges
			    self.remove_parallel_edges(id, k)
		    for k in n.get_parent_ids():
			    # remove inward edges
			    self.remove_parallel_edges(k, id)
		    self.nodes.pop(id, None)
	    else:
		    raise KeyError("No node whose id = {}".format(id))





