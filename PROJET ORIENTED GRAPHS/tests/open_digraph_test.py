import sys
import os
root = os.path.normpath(os.path.join(__file__, './../..'))
sys.path.append(root)# allows us to fetch files from the project root
import unittest
from modules.open_digraph import *

class InitTest(unittest.TestCase):
    def test_init_node(self):
        n0 = node(0, 'i', {}, {1:1})
        self.assertEqual(n0.id, 0)
        self.assertEqual(n0.label, 'i')
        self.assertEqual(n0.parents, {})
        self.assertEqual(n0.children, {1:1})
        self.assertIsInstance(n0, node)

    def test_init_digraph(self):
        n0 = node(0, 'a', {3:1, 4:1}, {1:1, 2:1})
        n1 = node(1, 'b', {0:1}, {2:2, 5:1})
        n2 = node(2, 'c', {0:1, 1:2}, {6:1})
        i0 = node(3, 'i0', {}, {0:1})
        i1 = node(4, 'i1', {}, {0:1})
        o0 = node(5, 'o0', {1:1}, {})
        o1 = node(6, 'o1', {2:1}, {})
        G = open_digraph([3,4], [5,6], [n0,n1,n2,i0,i1,o0,o1])
        self.assertEqual(G.inputs, [3,4])
        self.assertEqual(G.outputs, [5,6])
        self.assertEqual(G.nodes, {0:n0, 1:n1, 2:n2, 3:i0, 4:i1, 5:o0, 6:o1})
        self.assertEqual(G.next_id , 7)

class NodeTest(unittest.TestCase):
    def setUp(self):
        self.n0 = node(0, 'a', {}, {1:0})
        self.n1 = node(1, 'b', {0:1}, {2:2})
        self.n2 = node(2, 'c', {0:1, 1:2}, {})
        
    # Tests Getters

    def test_get_id(self):
        self.assertEqual(self.n0.get_id(), 0)

    def test_get_label(self):
        self.assertEqual(self.n0.get_label(), 'a')
        
    def test_get_parents(self):
        self.assertEqual(self.n0.get_parents(),{})
        self.assertEqual(self.n2.get_parents(),{0:1, 1:2})
        
    def test_get_parent_ids(self):
        self.assertEqual(self.n0.get_parent_ids(),[])
        self.assertEqual(self.n2.get_parent_ids(),[0,1])
    
    def test_get_children(self):
        self.assertEqual(self.n0.get_children(),{1:0})
        self.assertEqual(self.n2.get_children(),{})
        
    def test_get_children_ids(self):
        self.assertEqual(self.n0.get_children_ids(),[1])
        self.assertEqual(self.n2.get_children_ids(),[])
        
    # Tests Méthodes
    def test_copy(self):
        self.assertIsNot(self.n0.copy(),self.n0)

    def test_add_parent_id(self):
        self.assertEqual(self.n0.get_parent_ids(), [])
        self.n0.add_parent_id(3)
        self.assertEqual(self.n0.get_parent_ids(), [3])
        self.assertEqual(self.n2.get_parent_ids(), [0,1])
        self.n2.add_parent_id(4)
        self.assertEqual(self.n2.get_parent_ids(), [0,1,4])

    def test_add_child_id(self):
        self.assertEqual(self.n0.get_children_ids(), [1])
        self.n0.add_child_id(3)
        self.assertEqual(self.n0.get_children_ids(), [1,3])
        self.assertEqual(self.n2.get_children_ids(), [])
        self.n2.add_child_id(4)
        self.assertEqual(self.n2.get_children_ids(), [4])
        
    def test_degree_methods(self) : # tests of the following methods : in(out)degree and degree
        self.assertEqual(self.n1.indegree(),1)
        self.assertEqual(self.n1.outdegree(),2)
        self.assertEqual(self.n1.degree(),3)
        self.assertEqual(self.n0.indegree(),0)
        self.assertEqual(self.n0.outdegree(),0)
        self.assertEqual(self.n0.degree(),0)
        self.assertEqual(self.n2.outdegree(),0)      
        self.assertEqual(self.n2.degree(),3)        

class Open_digraphTest(unittest.TestCase):
    def setUp(self):
        self.n0 = node(0, 'a', {3:1, 4:1}, {1:1, 2:1})
        self.n1 = node(1, 'b', {0:1}, {2:2, 5:1})
        self.n2 = node(2, 'c', {0:1, 1:2}, {6:1})
        self.i0 = node(3, 'i0', {}, {0:1})
        self.i1 = node(4, 'i1', {}, {0:1})
        self.o0 = node(5, 'o0', {1:1}, {})
        self.o1 = node(6, 'o1', {2:1}, {})
        self.G = open_digraph([3,4], [5,6], [self.n0,self.n1,self.n2,self.i0,self.i1,self.o0,self.o1])
        
    # Tests Getters

    def test_get_input_ids(self):
        self.assertEqual(self.G.get_input_ids(), [3,4])

    def test_get_output_ids(self):
        self.assertEqual(self.G.get_output_ids(), [5,6])

    def test_get_id_node_map(self):
        self.assertEqual(self.G.get_id_node_map(), {0:self.n0,1:self.n1,2:self.n2,3:self.i0,4:self.i1,5:self.o0,6:self.o1})

    def test_get_nodes(self):
        self.assertEqual(self.G.get_nodes(), [self.n0,self.n1,self.n2,self.i0,self.i1,self.o0,self.o1])

    def test_get_node_ids(self):
        self.assertEqual(self.G.get_node_ids(), [0,1,2,3,4,5,6])

    def test_get_node_by_id(self):
        self.assertEqual(self.G.get_node_by_id(1), self.n1)

    def test_get_nodes_by_ids(self):
        self.assertEqual(self.G.get_nodes_by_ids([3,5,0]), [self.i0, self.o0, self.n0])

    
    # Tests Méthodes

    def test_copy(self):
        self.assertIsNot(self.G.copy(), self.G)

    def test_new_id(self):
        i = self.G.new_id()
        assert not i in self.G.get_node_ids()

    def test_add_edge(self):
        self.assertEqual(self.n1.get_parent_ids(), [0])
        self.assertEqual(self.i0.get_children_ids(), [0])
        self.G.add_edge(3,1)
        self.assertEqual(self.n1.get_parent_ids(), [0,3])
        self.assertEqual(self.i0.get_children_ids(), [0,1])

    def test_add_node(self):
        id_n = self.G.add_node('f', {3:1,4:1}, {1:2})
        self.assertEqual(self.i0.get_children()[id_n], 1)
        self.assertEqual(self.i1.get_children()[id_n], 1)
        self.assertEqual(self.n1.get_parents()[id_n], 2)

    def test_is_well_formed(self):
        """Test de la méthode is_well_formed de la classe open_digraph"""
        #Test de la validité du graphe de la fonction setUp
        self.assertEqual(self.G.is_well_formed(), True)
        
        #Test de la première condition : id des outputs et inputs dans le graphe
        G0 = open_digraph([3], [5], [self.n0,self.n1,self.n2,self.i0,self.i1,self.o0,self.o1])
        G1 = open_digraph([3,4,7], [5,6], [self.n0,self.n1,self.n2,self.i0,self.i1,self.o0,self.o1])
        G2 = open_digraph([3,4], [5,6,7], [self.n0,self.n1,self.n2,self.i0,self.i1,self.o0,self.o1])
        self.assertEqual(G0.is_well_formed(), True)
        self.assertEqual(G1.is_well_formed(), False)
        self.assertEqual(G2.is_well_formed(), False)

        #Test de la deuxième condition : noeuds inputs
        ## Vérification multiplicité du fils qui vaut 1
        i0 = node(3, 'i0', {}, {0:2})
        n0 = node(0, 'a', {3:2, 4:1}, {1:1, 2:1})
        G3 = open_digraph([3,4], [5,6], [n0,self.n1,self.n2,i0,self.i1,self.o0,self.o1])
        self.assertEqual(G3.is_well_formed(), False)
        ## Vérification unicité du fils
        i0 = node(3, 'i0', {}, {0:1, 1:1})
        n0 = node(0, 'a', {3:1, 4:1}, {1:1, 2:1})
        n1 = node(1, 'b', {3:1, 0:1}, {2:2, 5:1})
        G4 = open_digraph([3,4], [5,6], [n0,n1,self.n2,i0,self.i1,self.o0,self.o1])
        ## Vérification pas de parents
        self.assertEqual(G4.is_well_formed(), False)
        i0 = node(3, 'i0', {2:2}, {0:1})
        n2 = node(2, 'c', {0:1, 1:2}, {6:1,3:2})
        G5 = open_digraph([3,4], [5,6], [self.n0,self.n1,n2,i0,self.i1,self.o0,self.o1])
        self.assertEqual(G5.is_well_formed(), False)

        #Test de la troisième condition : noeuds outputs
        ## Vérification multiplicité du parent vaut 1
        o0 = node(5, 'o0', {1:2}, {})
        n1 = node(1, 'b', {0:1}, {2:2, 5:2})
        G6 = open_digraph([3,4], [5,6], [self.n0,n1,self.n2,self.i0,self.i1,o0,self.o1])
        self.assertEqual(G6.is_well_formed(), False)
        ## Vérification unicité des parents
        o0 = node(5, 'o0', {1:1, 2:1}, {})
        n1 = node(1, 'b', {0:1}, {2:2, 5:1})
        n2 = node(2, 'c', {0:1, 1:2}, {6:1, 5:1})
        G7 = open_digraph([3,4], [5,6], [self.n0,n1,n2,self.i0,self.i1,o0,self.o1])
        self.assertEqual(G7.is_well_formed(), False)
        ## Vérification pas d'enfants
        o0 = node(5, 'o0', {1:2}, {2:1})
        n2 = node(2, 'c', {0:1, 1:2, 5:1}, {6:1})
        G8 = open_digraph([3,4], [5,6], [self.n0,self.n1,n2,self.i0,self.i1,o0,self.o1])
        self.assertEqual(G8.is_well_formed(), False)

        #Test de la cinquième condition : égalité de la multiplicité entre noeuds enfants et parents
        n0 = node(0, 'a', {3:1, 4:1}, {1:1, 2:1})
        n1 = node(1, 'b', {0:2}, {2:2, 5:1})
        G9 = open_digraph([3,4], [5,6], [n0,n1,self.n2,self.i0,self.i1,self.o0,self.o1])
        self.assertEqual(G9.is_well_formed(), False)

        n0 = node(0, 'a', {3:3, 4:1}, {1:1, 2:1})
        i0 = node(3, 'i0', {}, {0:1})
        G10 = open_digraph([3,4], [5,6], [n0,self.n1,self.n2,i0,self.i1,self.o0,self.o1])
        self.assertEqual(G10.is_well_formed(), False)

    def test_add_and_remove_node(self):
        """Tests des méthodes qui ajoutent et retirent des noeuds"""
        self.assertEqual(self.G.is_well_formed(), True)
        new_id = self.G.add_node('d', {0:1}, {2:2})
        new_id_bis = self.G.add_node('e', {new_id:1}, {})
        self.assertEqual(new_id in self.G.get_node_ids(),True)
        self.assertEqual(self.G.is_well_formed(), True)
        self.G.remove_nodes_by_id(new_id,new_id_bis,rem_iso=False)
        self.assertEqual(self.G.is_well_formed(), True)
        self.assertEqual(new_id in self.G.get_node_ids(),False)
        self.assertEqual(new_id_bis in self.G.get_node_ids(),False)

    def test_add_and_remove_edge(self):
        """Tests des méthodes qui ajoutent et retirent des arêtes"""
        self.assertEqual(self.G.is_well_formed(), True)
        new_id = self.G.add_node('d', {0:1}, {2:2})
        self.G.remove_edge(1,2)
        self.G.remove_parallel_edges(new_id,2)
        self.G.remove_edge(0,new_id)
        self.assertEqual(new_id in self.G.get_node_ids(),False)
        self.assertEqual(self.G.is_well_formed(), True)
        self.G.add_edges([(1,2),(1,2),(0,1),(0,2)])
        self.assertEqual(self.G.is_well_formed(), True)

    def test_add_outputs_and_inputs(self):
        """Tests des méthodes qui ajoutent des inputs ou des ouputs"""
        self.assertEqual(self.G.is_well_formed(), True)
        i,o,d = len(self.G.inputs), len(self.G.outputs) , len(self.G.get_nodes())
        self.G.add_input_node(0)
        self.G.add_input_node(1)
        self.G.add_input_node(2)
        self.G.add_output_node(0)
        self.G.add_output_node(1)
        self.G.add_output_node(2)
        self.assertEqual(len(self.G.inputs),i+3)
        self.assertEqual(len(self.G.outputs),o+3)
        self.assertEqual(len(self.G.get_nodes()),d+6)
        self.assertEqual(self.G.is_well_formed(), True)

    def test_random(self):
        """Tests de la méthode random"""
        GG = open_digraph.random(4,3,inputs=3,outputs=2,DAG=True)
        #self.assertEqual(len(GG.get_nodes()), 9) peut être faux si un noeud est isolé
        self.assertEqual(GG.is_well_formed(), True)
        G1 = open_digraph.random(8,10,inputs=2,outputs=4,undirected=True)
        #self.assertEqual(len(G1.get_nodes()), 14) peut être faux si un noeud est isolé
        self.assertEqual(G1.is_well_formed(), True) 
        M = random_oriented_int_matrix(5,5)
        GG = open_digraph.empty()
        GG.graph_from_adjacency_matrix(M)
        self.assertEqual(M,GG.adjacency_matrix())
        
    def test_is_cyclic(self):
        """Test de la méthode is_cyclic"""
        self.assertFalse(self.G.is_cyclic())
        GG = open_digraph.random(4,4,DAG=True,loop_free=False)
        self.assertFalse(GG.is_cyclic())       
      
    def test_min_and_max_id(self):
        """Test des méthodes min_id et max_id"""
        self.assertEqual(self.G.max_id(), 6)
        self.assertEqual(self.G.min_id(), 0)

    def test_shift_indices(self):
        """Test de la méthode shift_indices"""
        ids = self.G.get_node_ids()
        n = len(ids)
        self.G.shift_indices(10)
        new_ids = self.G.get_node_ids()
        new_nodes = self.G.get_nodes()
        for i in range(n) :
            self.assertEqual(ids[i]+10,new_ids[i])
        for node in new_nodes :
            self.assertTrue(node.get_id() in new_ids)
            parents_ids = node.get_parent_ids()
            children_ids = node.get_children_ids()
            for parent_id in parents_ids :
                self.assertTrue(parent_id in new_ids)
            for child_id in children_ids :
                self.assertTrue(child_id in new_ids)
        self.G.shift_indices(-10)
        new_ids = self.G.get_node_ids()
        for i in range(len(ids)) :
            self.assertEqual(ids[i],new_ids[i])

    def test_parallel_methods(self):
        """Tests des méthodes iparallel et parallel"""
        G0 = self.G.parallel(open_digraph.empty())
        self.assertEqual(len(G0.get_node_ids()),len(self.G.get_node_ids()))
        G0 = open_digraph.empty().parallel(self.G)
        self.assertEqual(len(G0.get_node_ids()),len(self.G.get_node_ids()))
        for i in range(10):
            GG = open_digraph.random(4,3,inputs=10,outputs=10,DAG=True)
            self.G.iparallel(GG)
            self.assertEqual(self.G.is_well_formed(), True)
            G2 = open_digraph.random(5,3,inputs=1,outputs=2,DAG=False)
            G1 = self.G.parallel(G2)
            self.assertEqual(self.G.is_well_formed(), True)

    def test_compose_methods(self):
        """ Tests of the followings methods :
            - open_digraph.icompose
            - open_digraph.compose
            - open_digraph.identity
        """
        G0 = self.G.compose2(open_digraph.identity(2))
        self.assertEqual(len(G0.get_node_ids()),len(self.G.get_node_ids()))
        G0 = open_digraph.identity(2).compose2(self.G)
        self.assertEqual(len(G0.get_node_ids()),len(self.G.get_node_ids()))
        for i in range(10):
            with self.assertRaises(ValueError) as ctx :
                self.G.icompose(open_digraph.random(4,3,inputs=10,outputs=10,DAG=True))
            G2 = open_digraph.random(5,3,inputs=1,outputs=2,DAG=False)
            G1 = self.G.compose2(G2)
            self.assertEqual(self.G.is_well_formed(), True)
            G2 = open_digraph.random(5,3,inputs=1,outputs=2,DAG=False)
            self.G.compose2(G2)
            self.assertEqual(self.G.is_well_formed(), True)
        
    def test_connected_components_methods(self):
        """ Tests of the followings methods :
                - open_digraph.connected_components
                - open_digraph.get_connected_components
            """
        self.assertEqual(self.G.connected_components()[0],0)
        self.G.icompose(self.G)
        self.assertEqual(self.G.connected_components()[0],0)
        self.assertEqual(len(self.G.get_connected_components()),1)
        GG = self.G.parallel(self.G)
        self.assertEqual(GG.connected_components()[0],1)
        self.assertEqual(len(GG.get_connected_components()),2)
                   
    def test_Dijsktra(self):
        """ Tests de la méthode open_digraph.Dijsktra"""
        D1, D2 = self.G.Dijsktra(self.n1)
        self.assertEqual(D1[self.n1], 0)
        self.assertEqual(D1[self.n0], 1)
        self.assertEqual(D1[self.n2], 1)
        self.assertEqual(D1[self.o0], 1)
        self.assertEqual(D1[self.i0], 2)
        self.assertEqual(D1[self.i1], 2)
        self.assertEqual(D1[self.o1], 2)
        self.assertEqual(D2[self.n0], self.n1)
        self.assertEqual(D2[self.n2], self.n1)
        self.assertEqual(D2[self.o0], self.n1)
        self.assertEqual(D2[self.i0], self.n0)
        self.assertEqual(D2[self.i1], self.n0)
        self.assertEqual(D2[self.o1], self.n2)
        
    def test_tri_toplogique(self):
        """ Tests de la méthode open_digraph.tri_topologique"""
        l = self.G.tri_topologique()
        self.assertEqual(len(l),5)
        self.assertListEqual(l[0],[3,4])
        self.assertListEqual(l[1],[0])
        self.assertListEqual(l[2],[1])
        self.assertListEqual(l[3],[2,5])
        self.assertListEqual(l[4],[6])
    
    def test_profondeur(self):
        """ Tests of the following methods :
            - open_digraph.get_node_profondeur
            - open_digraph.get_profondeur
        """
        self.assertEqual(self.G.get_node_profondeur(3),0)
        self.assertEqual(self.G.get_node_profondeur(5),3)
        with self.assertRaises(ValueError) as ctx :
            self.G.get_node_profondeur(7)
        for i in range(5):
            GG = open_digraph.random(3,1,DAG=True)
            n= len(GG.tri_topologique())
            self.assertEqual(GG.get_profondeur(),n)
            self.assertTrue(GG.get_profondeur()<= 3)
        self.assertEqual(5,self.G.get_profondeur())
    
    def test_calcul_chemin_distance(self):
        """ Tests de la methode open_digraph.calcul_chemin_distance"""
        self.assertEqual(self.G.calcul_chemin_distance(self.n0, self.n2)[0],2)
        self.assertEqual(self.G.calcul_chemin_distance(self.i1, self.n2)[0],3)
        self.assertEqual(self.G.calcul_chemin_distance(self.n1, self.o0)[0],1)
        self.assertEqual(self.G.calcul_chemin_distance(self.i0, self.o1)[0],4)
        with self.assertRaises(ValueError) as ctx :
            self.G.calcul_chemin_distance(self.i0, self.i1)
        with self.assertRaises(ValueError) as ctx :
            self.G.calcul_chemin_distance(node(0,"",[],[]),self.n0)
        with self.assertRaises(ValueError) as ctx :
            self.G.calcul_chemin_distance(self.n0,node(0,"",[],[]))
        id = self.G.add_node()
        n = self.G[id]
        with self.assertRaises(ValueError) as ctx :
            self.G.calcul_chemin_distance(self.n0,n)
                          
class Bool_CircTest(unittest.TestCase):
    
    def setUp(self):
        self.l = node(5, '', {}, {13:1})
        self.m = node(6, '', {}, {13:1})
        self.n = node(7, '0', {}, {9:1})
        self.o = node(8, '1', {}, {9:1})
        self.p = node(9, '&', {7:1,8:1}, {11:1})
        self.q = node(10, '1', {}, {11:1})
        self.r = node(11, '|', {9:1,10:1,13:1}, {12:1})
        self.s = node(12, '', {11:1}, {})
        self.t = node(13, '^' , {6:1,5:1},{11:1})
        self.G = open_digraph([5,6], [12], [self.l,self.m,self.n,self.o,self.p,self.q,self.r,self.s,self.t])
        self.NC = self.G.bool_circ(self.G)

    def test_is_well_formed(self):
        """ Tests of the following methods :
            - bool_circ.is_well_formed
            - bool_circ.is_well_formed_boolean
            - bool_circ.assert_is_well_formed
            """ 
        self.assertEqual(self.NC.is_well_formed(), (0,None))
        self.assertTrue(self.NC.is_well_formed_boolean())
        id = self.NC.graph.add_node('2',{},{13:1})
        self.assertEqual(self.NC.is_well_formed(),(2,id))
        self.assertFalse(self.NC.is_well_formed_boolean())
        with self.assertRaises(ValueError) as ctx :
            self.NC.assert_is_well_formed()
        self.NC.graph.remove_node_by_id(id)
        id = self.NC.graph.add_node("~",{7:3},{8:1})
        with self.assertRaises(ValueError) as ctx:
            self.NC.assert_is_well_formed()
        a = node(0, '&', {3:1}, {1:1,2:1})
        b = node(1, '1', {0:1}, {})
        c = node(2, '0', {0:1}, {4:1})
        d = node(3, '1', {}, {0:1})
        e = node(4, '1', {2:1}, {})
        A = open_digraph([3], [4], [a, b, c, d, e])
        with self.assertRaises(ValueError) as ctx :
            A.bool_circ(A)

    def test_bool_circuit(self):
        """ Tests de la méthode bool_circ.bool_circ"""
        for size in range(1,10):
            for inputs in range(1,10):
                for outputs in range(1,10):
                    G = open_digraph.bool_circ.bool_circ(size,inputs,outputs)
                    self.assertTrue(G.is_well_formed_boolean())
                    self.assertEqual(len(G.graph.get_input_ids()),inputs)
                    self.assertEqual(len(G.graph.get_output_ids()),outputs)

    # La méthode open_digraph.bool_circ.calcul fait office de fonction test pour les méthodes suivantes :
    #   - méthodes adder
    #   - méthode from_int_to_registre
    #   - méthodes de simplifications
    #   - méthodes evaluate

    def test_calcul(self):
        puissance_taille_registre = 1
        for i in range(0,2**(2**(puissance_taille_registre+1)+1)):
            open_digraph.bool_circ.calcul(puissance_taille_registre,i)

    # De même, la méthode.bool_circ.verificationHamming fait office de test pour les méthodes suivantes :
    #   - méthodes encodeur et décodeur
    #   - méthodes de réécriture
    #   - méthode evaluate2

    def test_verificationHamming(self):
        open_digraph.bool_circ.verificationHamming([],allNumbers=True)

    def test_profondeur(self):
        "Test fonctions profondeur"
        for i in range(5):
            G = open_digraph.bool_circ.adder(i)
            G.profondeur(i, G.get_graph().get_input_ids(), G.get_graph().get_output_ids())

if __name__ == '__main__': # the following code is called only when
    unittest.main() # precisely this file is run