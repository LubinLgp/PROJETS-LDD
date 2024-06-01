import sys
import os
root = os.path.normpath(os.path.join(__file__, './../..'))
sys.path.append(root)  # allows us to fetch files from the project root
import os.path
import re
import webbrowser

from modules.mixins.graph.architecture import *

class Affichage(Architecture):
    def node_type(self, nid: int):
        """ Cette fonction est un raccourci pour savoir quel est le type du noeud
        à savoir 'o' pour output, 'i' pour input et 'n' pour simple noeud
        Inputs:
            - nid (entier): id du noeud auquel on s'interesse
        Outputs:
            - type du noeud
            - éventuellement 'None' si le noeud n'existe pas
        """
        if nid in self.get_node_ids():
            if nid in self.get_input_ids():
                return 'i'
            elif nid in self.get_output_ids():
                return 'o'
            else:
                return 'n'
        else:
            return None

    def save_as_dot_file(self, path: str, verbose=False) -> bool:
        """ Enregistre le graphe sous format '.dot' à l'endroit
        spécifié par path.
        Inputs:
            - path (str) non-vide: qui spécifie le chemin ou le graphe doit être sauvegardé
            - verbose (bool): permet de spécifier si on veut afficher les [ids] en
            même temps que les [labels]
        Returns :
            bool : l'argument verbose
        May raise:
            - IOError
            - OSError
            - FileNotFoundError
            - PermissionError
            - OSError
        Le formatage du fichier.dot est le suivant :
            - On commence la déclaration du graph sur la première ligne via 'diagraph G {\n'
            - Chaque instruction commence par '\t' et finit par un ";\n"
            - Le premier caractère de chaque instruction est dans {'n','i','o'} dépendant sur qui porte l'instruction
            - Pour chaque noeud déclaré, on déclare son id ainsi que son label
            - Le numéro du noeud est exactement son id
            - On ne déclare pas plus d'une arète par instruction
            - On déclare en premier les noeuds n'étant pas (in)output puis les inputs et enfin les outputs
            - On fini la déclaration du graph par '}'
        """
        try:
            f = open(path + ".dot", 'w+')
            l = ["digraph G {\n\n\t"]
            for n in self.get_nodes():
                nid = n.get_id()
                s = ""
                lb = ""
                shape = ""
                if nid in self.get_input_ids():
                    # le noeud est un input
                    s = "i{}".format(nid)
                    lb = "xlabel"
                    shape = "shape = point"
                elif nid in self.get_output_ids():
                    # le noeud est un output
                    s = "o{}".format(nid)
                    lb = "xlabel"
                    shape = "shape = point"
                else:
                    # noeud intermediaire
                    s = "n{}".format(nid)
                    lb = "label"
                if verbose:
                    lb = lb + ' = "({}, {})"'.format(n.get_label(), nid)
                else:
                    lb = lb + ' = "{}"'.format(n.get_label())
                l.append("{} [id = {} {} {}];\n\t".format(s, nid, lb, shape))

                # ajout de toutes les arêtes a tous les enfants
                for k in n.get_children():
                    t = self.node_type(k)
                    if t is not (None):
                        for i in range(n.get_children()[k]):
                            l.append("{} -> {}{} [arrowhead = vee];\n\t".format(s, t, k))
            l.append("\n}\n")
            try:
                f.writelines(l)
            except (IOError, OSError):
                print("Erreur lors de l'ecriture dans le fichier.\n")
                f.close()
            f.close()
        except (FileNotFoundError, PermissionError, OSError):
            print("Erreur lors de l'ouveture du fichier.")
        return verbose

    @classmethod
    def from_dot_file(cls, path: str):  # -> open_digraph
        """ Retourne le graph formé à partir du fichier '.dot' dont l'emplacement est [path]
        Args:
            path (str): emplacement du fichier '.dot'
        Raises:
            FileNotFoundError: si le chemin [path] ne réfère pas à un fichier existant
            ValueError: si le chemin [path] ne réfère pas à un '.dot'
            ValueError: si le fichier ne respecte pas les règles de formatage
        Returns:
            open_digraph: graph obtenu à partir du fichier '.dot'
         """
        if not os.path.exists(path):
            raise FileNotFoundError("Chemin inexistant")
        if not (os.path.splitext(path)[-1] == ".dot"):
            raise ValueError("Le chemin ne refère pas à un'.dot'")
        f = open(path, "r")
        lines = f.readlines()
        n = len(lines)
        if not lines[0] != "digraph G { \n" or lines[n - 1] != "}\n":
            raise ValueError("Formatage incorrect")
        inputs = list()
        outputs = list()
        nodes = dict()
        for i in range(2, n - 2):
            f_char = lines[i][1]
            if f_char not in {'i', 'o', 'n'}:
                raise ValueError("Formatage incorrect")
            l = re.findall('id = ([0-9]+) .?label = "(.+)" ', lines[i])
            if (len(l)) != 0:
                nod = node(int(l[0][0]), str(l[0][1]), {}, {})
                if f_char == 'i':
                    inputs.append(nod.get_id())
                if f_char == 'o':
                    outputs.append(nod.get_id())
                nodes[nod.get_id()] = nod
        for i in range(2, n - 2):
            l = re.findall('\t.{1}(.+) -> .{1}([0-9]+)', lines[i])
            if (len(l)) != 0:
                if (len(l[0])) != 2:
                    raise ValueError("Formatage incorrect")
                src = nodes[int(l[0][0])]
                tgt = nodes[int(l[0][1])]
                src.add_child_id(tgt.get_id())
                tgt.add_parent_id(src.get_id())
        G = cls(inputs, outputs, nodes.values())
        G.assert_is_well_formed()
        f.close()
        return G

    def display(self, verbose=False):
        """ Affiche le graphique sur un visualisateur en ligne
        Args:
            verbose (bool) : spécifie si on veut afficher les [ids] en
            même temps que les [labels]
        """
        self.save_as_dot_file("tmp", verbose)
        f = open("tmp.dot", "r")
        lines = f.readlines()
        txt = "#"
        for line in lines:
            for carac in line[:-1]:
                if carac == ";":
                    txt += "%3B"
                else:
                    txt += carac
            txt += "%0A%09"
        webbrowser.open("https://dreampuf.github.io/GraphvizOnline/" + txt)
        f.close()
        try:
            os.system("rm -f tmp.dot")  # sous Unix
        except:
            print("Impossible de supprimer le fichier 'tmp.dot', veuillez le supprimer manuellement")