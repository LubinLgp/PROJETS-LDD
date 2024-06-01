open Random
(* Fonction récursive pour afficher le contenu d'un fichier ligne par ligne *)
let rec dump_file f =
  try 
    Printf.printf "%s \n" (input_line f) ;
    dump_file f
  with End_of_file -> ()

(* Définition des différents types de cases possibles dans le labyrinthe *)
type casetype = Vide | Mur | Entree | Sortie | Chemin

(* Définition de la structure d'une case du labyrinthe *)
type cellule = { ligne : int; colonne : int; mutable case : casetype } (*mutable veut juste dire qu'on peut modifier le type d'une case après l'avoir initalisé*)

(* Définition des différentes directions possibles *)
type direction = Haut | Bas | Gauche | Droite

(* Définition de la structure du labyrinthe *)
type labyrinthe = { grille : cellule array array; nbLigne : int; nbColonne : int }

(* Fonction pour convertir un caractère en type de case *)
let char_to_case c i j =
  match c with
  | '|' | '-' | '+' -> { ligne = j; colonne = i; case = Mur }
  | '.' -> { ligne = j; colonne = i; case = Chemin }
  | 'E' -> { ligne = j; colonne = i; case = Entree }
  | 'S' -> { ligne = j; colonne = i; case = Sortie }
  | ' ' -> { ligne = j; colonne = i; case = Vide }
  | _ -> failwith "Erreur (caractère inconnu)" (* Gestion d'un caractère inconnu *)

(* Fonction pour convertir un fichier en labyrinthe *)
let file_to_laby c_in =
  let rec loop acc acc2 =
    try
      let s = input_line c_in in
      loop (s :: acc) (acc2 + 1)
    with
    | End_of_file -> (List.rev acc, acc2)
  in
  let res = loop [] 0 in
  let list_ligne = fst res in
  let nbLigne = snd res in

  let nbColonne =
    match list_ligne with
    | [] -> 0 (* Gestion d'une liste vide *)
    | hd :: _ -> String.length hd
  in

  let explode s numLigne =
    Array.init (String.length s) (fun i -> char_to_case s.[i] i numLigne)
  in

  let grille =
    Array.of_list (List.mapi (fun i line -> explode line i) list_ligne)
  in

  { grille = grille; nbLigne = nbLigne; nbColonne = nbColonne }

(* Fonction pour afficher une case du labyrinthe *)
let affiche_case c =
  match c.case with 
    | Vide -> Printf.printf " "
    | Mur -> if c.ligne mod 2 = 0  then
              if c.colonne mod 2 = 0 then Printf.printf "+"
              else Printf.printf "-"
            else
              Printf.printf "|"
    | Entree -> Printf.printf  "E"
    | Sortie -> Printf.printf "S"
    | Chemin -> Printf.printf "\027[31m.\027[0m" (* Cette chaîne de caractères affiche un "." en rouge *)

(* Fonction pour afficher un tableau de cases *)
let affiche_array a =
  Array.iter (fun c -> affiche_case c) a

(* Fonction pour afficher un tableau 2D de cases *)
let affiche_double_array a =
  Array.iter (fun a1 -> affiche_array a1; Printf.printf "\n") a

(* Fonction pour vérifier s'il y a une seule entrée et une seule sortie dans le labyrinthe *)
let check_entree_sortie_unique labyrinthe =
  let count_type casetype =
    Array.fold_left
      (fun acc row ->
        acc + Array.fold_left (fun acc' cell -> if cell.case = casetype then acc' + 1 else acc') 0 row)
      0 labyrinthe.grille
  in
  let entree_count = count_type Entree in
  let sortie_count = count_type Sortie in
  entree_count = 1 && sortie_count = 1

(* Fonction pour vérifier si le labyrinthe a le bon format *)
let check_labyrinthe_format labyrinthe =
  let longueur_premiere_ligne =
    if labyrinthe.nbLigne > 0 then
      Array.length labyrinthe.grille.(0)
    else
      0
  in
  let meme_longueur_partout =
    Array.fold_left
      (fun acc row -> acc && Array.length row = longueur_premiere_ligne)
      true labyrinthe.grille
  in
  meme_longueur_partout

(* Fonction pour vérifier si le labyrinthe est entouré de murs *)
let check_labyrinthe_entoure_murs labyrinthe =
  let est_mur c = c.case = Mur in

  let mur_audessus = Array.for_all est_mur labyrinthe.grille.(0) in
  let mur_endessous = Array.for_all est_mur labyrinthe.grille.(labyrinthe.nbLigne - 1) in

  let mur_cote =
    Array.fold_left
      (fun acc row ->
        acc && est_mur row.(0) && est_mur row.(Array.length row - 1))
      true labyrinthe.grille
  in

  mur_audessus && mur_endessous && mur_cote

(* Fonction pour vérifier si le labyrinthe est valide *)
let estValide labyrinthe = 
  check_labyrinthe_entoure_murs labyrinthe && check_labyrinthe_format labyrinthe && check_entree_sortie_unique labyrinthe 
(* Fonction pour déterminer les directions possibles à partir d'une position dans le labyrinthe *)
let direction_possible labyrinthe i j =
  let res = Array.make 4 false in
  if i >= 1 then
    res.(0) <- ((labyrinthe.grille.(i - 1)).(j)).case = Vide; (* HAUT *)
  if j < labyrinthe.nbColonne then
    res.(1) <- ((labyrinthe.grille.(i)).(j + 1)).case = Vide; (* droite *)
  if i < labyrinthe.nbLigne then
    res.(2) <- ((labyrinthe.grille.(i + 1)).(j)).case = Vide; (* bas *)
  if j >= 1 then
    res.(3) <- ((labyrinthe.grille.(i)).(j - 1)).case = Vide; (* gauche *)
  res

(* Fonction pour obtenir les voisins d'une position dans le labyrinthe *)
let get_neighbors ligne colonne lignes colonnes =
  let directions_possibles = [Haut; Bas; Gauche; Droite] in
  List.fold_left (fun acc dir ->
    let (dx, dy) =
      match dir with
      | Haut -> (-1, 0)
      | Bas -> (1, 0)
      | Gauche -> (0, -1)
      | Droite -> (0, 1)
    in
    let new_ligne = ligne + dx in
    let new_colonne = colonne + dy in
    if new_ligne >= 0 && new_ligne < lignes && new_colonne >= 0 && new_colonne < colonnes then
      (new_ligne, new_colonne) :: acc
    else
      acc
  ) [] directions_possibles

(* Fonction pour trouver le point de départ dans le labyrinthe *)
let trouver_point_depart labyrinthe =
  let lignes = Array.length labyrinthe.grille in
  let colonnes = Array.length labyrinthe.grille.(0) in
  let rec trouver_depart ligne colonne =
    if ligne >= lignes then None
    else if colonne >= colonnes then trouver_depart (ligne + 1) 0
    else
      match labyrinthe.grille.(ligne).(colonne).case with
      | Sortie -> Some (ligne, colonne)
      | _ -> trouver_depart ligne (colonne + 1)
  in
  trouver_depart 0 0

(* Fonction pour résoudre le labyrinthe *)
let resoudre_labyrinthe labyrinthe =
  let lignes = Array.length labyrinthe.grille in
  let colonnes = Array.length labyrinthe.grille.(0) in

  let visite = Array.make_matrix lignes colonnes false in
  let chemin = ref [] in

  let rec resoudre ligne colonne =
    if ligne < 0 || colonne < 0 || ligne >= lignes || colonne >= colonnes then false
    else if visite.(ligne).(colonne) || labyrinthe.grille.(ligne).(colonne).case = Mur then false
    else if labyrinthe.grille.(ligne).(colonne).case = Entree then (
      chemin := (ligne, colonne) :: !chemin;
      true
    )
    else (
      visite.(ligne).(colonne) <- true;
      chemin := (ligne, colonne) :: !chemin;

      let voisins = get_neighbors ligne colonne lignes colonnes in
      let rec boucle = function
        | [] -> false
        | (l, c) :: reste ->
          if resoudre l c then true
          else boucle reste
      in
      let trouve = boucle voisins in

      if not trouve then chemin := List.tl !chemin;
      trouve
    )
  in

  match trouver_point_depart labyrinthe with
  | Some (ligne_depart, colonne_depart) ->
    let resultat = resoudre ligne_depart colonne_depart in
    if resultat then Some (List.rev !chemin)
    else None
  | None -> None

(* Fonction pour afficher les coordonnées du chemin dans le labyrinthe *)
let afficher_coordonnees_chemin chemin =
  List.iter (fun (ligne, colonne) ->
    Printf.printf "(%d, %d) -> " ligne colonne
  ) chemin;
  Printf.printf "Fin\n"

(* Fonction de test pour résoudre le labyrinthe et afficher le chemin trouvé ou un message s'il n'y a pas de chemin *)
let tester_resoudre_labyrinthe labyrinthe =
  match resoudre_labyrinthe labyrinthe with
  | Some chemin ->
    Printf.printf "Chemin trouvé : ";
    afficher_coordonnees_chemin chemin
  | None -> Printf.printf "Pas de chemin trouvé\n"

(* Fonction pour mettre à jour le labyrinthe avec le chemin trouvé *)
let update_labyrinthe_avec_chemin labyrinthe coordonnees =
  match coordonnees with
  | Some coords ->
    let nouveau_labyrinthe = {labyrinthe with grille = Array.map Array.copy labyrinthe.grille} in
    List.iteri (fun i (ligne, colonne) ->
      if i > 0 && i < List.length coords - 1 then
        nouveau_labyrinthe.grille.(ligne).(colonne).case <- Chemin
    ) coords;
    nouveau_labyrinthe
  | None -> labyrinthe

(* Fonction pour créer un labyrinthe vide avec le nombre de lignes et de colonnes spécifié *)
let creer_labyrinthe_vide lignes colonnes =
  let rec initialiser_ligne ligne colonne =
    if colonne >= colonnes then []
    else
      {
        ligne = ligne;
        colonne = colonne;
        case = if ligne mod 2 = 0 || colonne mod 2 = 0 then Mur else Vide
      } :: initialiser_ligne ligne (colonne + 1)
  in
  let rec initialiser_lignes ligne =
    if ligne >= lignes then []
    else
      Array.of_list (initialiser_ligne ligne 0) :: initialiser_lignes (ligne + 1)
  in
  let grille = Array.of_list (initialiser_lignes 0) in
  { grille; nbLigne = lignes; nbColonne = colonnes }

(* Fonction pour générer un labyrinthe aléatoire avec le nombre de lignes, de colonnes et une graine spécifiés *)
let generer_labyrinthe_aleatoire lignes colonnes graine =
  Random.init graine;
  if lignes = 1 && colonnes = 1 then failwith "Le labyrinthe ne peut pas être de taille 1x1";
  
  let lignes = (2 * lignes) + 1 in
  let colonnes = (2 * colonnes) + 1 in

  let labyrinthe = creer_labyrinthe_vide lignes colonnes in

  let visite = Array.make_matrix lignes colonnes false in

  let rec visiter ligne colonne =
    visite.(ligne).(colonne) <- true;

    let directions = [Haut; Bas; Gauche; Droite] |> List.sort (fun _ _ -> Random.int 3 - 1) in

    List.iter (fun direction ->
      match direction with
      | Haut ->
        if ligne > 2 && not visite.(ligne - 2).(colonne) then (
          labyrinthe.grille.(ligne).(colonne).case <- Vide;
          labyrinthe.grille.(ligne - 1).(colonne).case <- Vide;
          visiter (ligne - 2) colonne
        )
      | Bas ->
        if ligne < lignes - 3 && not visite.(ligne + 2).(colonne) then (
          labyrinthe.grille.(ligne).(colonne).case <- Vide;
          labyrinthe.grille.(ligne + 1).(colonne).case <- Vide;
          visiter (ligne + 2) colonne
        )
      | Gauche ->
        if colonne > 2 && not visite.(ligne).(colonne - 2) then (
          labyrinthe.grille.(ligne).(colonne).case <- Vide;
          labyrinthe.grille.(ligne).(colonne - 1).case <- Vide;
          visiter ligne (colonne - 2)
        )
      | Droite ->
        if colonne < colonnes - 3 && not visite.(ligne).(colonne + 2) then (
          labyrinthe.grille.(ligne).(colonne).case <- Vide;
          labyrinthe.grille.(ligne).(colonne + 1).case <- Vide;
          visiter ligne (colonne + 2)
        )
    ) directions
  in

  let (ligne_entree, colonne_entree) = (Random.int (lignes / 2)) * 2 + 1, (Random.int (colonnes / 2)) * 2 + 1 in
  
  let rec position_aleatoire_differentes_entree () =
    let ligne = (Random.int (lignes / 2)) * 2 + 1 in
    let colonne = (Random.int (colonnes / 2)) * 2 + 1 in
    if (ligne, colonne) = (ligne_entree, colonne_entree) then
      position_aleatoire_differentes_entree ()
    else
      (ligne, colonne)
  in

  let (ligne_sortie, colonne_sortie) = position_aleatoire_differentes_entree () in

  visiter ligne_entree colonne_entree;

  labyrinthe.grille.(ligne_entree).(colonne_entree).case <- Entree;
  labyrinthe.grille.(ligne_sortie).(colonne_sortie).case <- Sortie;

  labyrinthe


(* Fonction pour vérifier si la case est un mur *)
let est_mur c = c.case = Mur

(* Fonction pour vérifier la présence de murs à gauche et à droite d'une case *)
let mur_gauche_droite grille ligne colonne =
  let mur_gauche = if colonne - 1 >= 0 then est_mur grille.(ligne).(colonne - 1) else false in
  let mur_droite = if colonne + 1 < Array.length grille.(ligne) then est_mur grille.(ligne).(colonne + 1) else false in
  mur_gauche, mur_droite

(* Fonction pour vérifier la présence de murs en haut et en bas d'une case *)
let mur_haut_bas grille ligne colonne =
  let mur_haut = if ligne - 1 >= 0 then est_mur grille.(ligne - 1).(colonne) else false in
  let mur_bas = if ligne + 1 < Array.length grille then est_mur grille.(ligne + 1).(colonne) else false in
  mur_haut, mur_bas

(* Fonction pour vérifier si la case est un chemin *)
let est_chemin c = c.case = Chemin

(* Fonction pour vérifier la présence de chemins à gauche et à droite d'une case *)
let chemin_gauche_droite grille ligne colonne=
  let chemin_gauche = if colonne - 1 >= 0 then est_chemin grille.(ligne).(colonne - 1) else false in
  let chemin_droite = if colonne + 1 < Array.length grille.(ligne) then est_chemin grille.(ligne).(colonne + 1) else false in
  chemin_gauche, chemin_droite

(* Fonction pour vérifier la présence de chemins en haut et en bas d'une case *)
let chemin_haut_bas grille ligne colonne =
  let chemin_haut = if ligne - 1 >= 0 then est_chemin grille.(ligne - 1).(colonne) else false in
  let chemin_bas = if ligne + 1 < Array.length grille then est_chemin grille.(ligne + 1).(colonne) else false in
  chemin_haut, chemin_bas

(* Fonction pour afficher le labyrinthe de manière stylisée *)
let affiche_pretty laby =
  let affiche_case_pretty c =
    match c.case with
    | Vide -> Printf.printf " "
    | Mur ->
	    let mur_hb = mur_haut_bas laby.grille c.ligne c.colonne in
		let mur_gd = mur_gauche_droite laby.grille c.ligne c.colonne in
        if mur_hb = (false, false) && mur_gd = (false, false) then Printf.printf "╋"
		else if mur_hb = (true, true) && mur_gd = (true, true) then Printf.printf "╋"
        else if mur_hb = (false, true) && mur_gd = (true, false) then Printf.printf "┓"
        else if mur_hb = (false, true) && mur_gd = (false, true) then Printf.printf "┏"
        else if mur_hb = (true, false) && mur_gd = (true, false) then Printf.printf "┛"
        else if mur_hb = (true, false) && mur_gd = (false, true) then Printf.printf "┗"
        else if mur_hb = (false, false) && mur_gd = (true, true) then Printf.printf "━"
        else if mur_hb = (true, true) && mur_gd = (false, false) then Printf.printf "┃"
        else if mur_hb = (true, true) && mur_gd = (true, false) then Printf.printf "┫"
        else if mur_hb = (true, true) && mur_gd = (false, true) then Printf.printf "┣"
        else if mur_hb = (true, false) && mur_gd = (true, true) then Printf.printf "┻"
        else if mur_hb = (false, true) && mur_gd = (true, true) then Printf.printf "┳"
        else Printf.printf " "
    | Entree -> Printf.printf "🚪"
    | Sortie -> Printf.printf "🚩"
    | Chemin ->
	    let chemin_hb = chemin_haut_bas laby.grille c.ligne c.colonne in
		let chemin_gd = chemin_gauche_droite laby.grille c.ligne c.colonne in
        if chemin_hb = (false, true) && chemin_gd = (true, false) then Printf.printf "\027[31m┐\027[0m"
        else if chemin_hb = (false, true) && chemin_gd = (false, true) then Printf.printf "\027[31m┌\027[0m"
        else if chemin_hb = (true, false) && chemin_gd = (true, false) then Printf.printf "\027[31m┘\027[0m"
        else if chemin_hb = (true, false) && chemin_gd = (false, true) then Printf.printf "\027[31m└\027[0m"
        else if chemin_hb = (false, false) || chemin_gd = (true, true) then Printf.printf "\027[31m─\027[0m"
        else if chemin_hb = (true, true) || chemin_gd = (false, false) then Printf.printf "\027[31m│\027[0m"
        else Printf.printf " "
  in

  let affiche_array_pretty a =
    Array.iter (fun c -> affiche_case_pretty c) a
  in

  let affiche_double_array_pretty a =
    Array.iter (fun a1 -> affiche_array_pretty a1; Printf.printf "\n") a
  in

  affiche_double_array_pretty laby.grille

(*fonction qui genere n Labyrinthes de taille l*L et verifie qu'ils sont tous valides (pour tester notre fonction generate_random_labyrinthe) *)
let verifie_generation n l l2 =
  let rec aux count =
    if count = n then
      true
    else
	  let seed = (abs (Random.bits ())) in
      let labyrinth = generer_labyrinthe_aleatoire l l2 seed in
      if estValide labyrinth then
        aux (count + 1)
      else begin
		affiche_double_array (labyrinth.grille);
        false
	  end
  in
  aux 0




(* Fonction pour afficher un message d'aide *)
let afficher_message_aide () =
  print_endline "Usage: maze.exe [commande] [arguments]";
  print_endline "Commandes:";
  print_endline "  maze.exe --help : Affiche un message d'aide sur les différentes options";
  print_endline "  maze.exe --test [n] [l] [L] : Genere n labyrinthes de taille l*L de façon aléatoire et vérifie qu'ils sont tous valides";
  print_endline "                                Par défaut, n=100 et l et L sont générés aléatoirement entre 0 et 99";
  print_endline "  maze.exe print <fichier.laby> : Lit et affiche le labyrinthe du fichier";
  print_endline "  maze.exe print --pretty <fichier.laby> : Lit et affiche le labyrinthe du fichier avec des caractères unicode de manière stylisée";
  print_endline "  maze.exe solve <fichier.laby> : Résout et affiche le chemin du labyrinthe du fichier";
  print_endline "  maze.exe solve --pretty <fichier.laby> : Résout et affiche le chemin du labyrinthe du fichier avec des caractères unicode de manière stylisée";
  print_endline "  maze.exe random <n> <m> [r] : Génère un labyrinthe aléatoire de taille n x m";
  print_endline "                                [r] est une graine aléatoire optionnelle";;

let () =
  Random.self_init ();
  match Array.to_list Sys.argv with
  | [ _; "--help" ] -> afficher_message_aide()
  | [ _; "--test";n;l;l2 ] -> if (verifie_generation (int_of_string n) (int_of_string l) (int_of_string l2)) then 
	  print_endline "Tous les labyrinthes générés sont valides" 
	else 
	  print_endline "Au moins un des labyrinthes générés n'est pas valide";exit 3
  | [ _; "--test"] -> if (verifie_generation 100 (Random.int 100) (Random.int 100)) then 
	  print_endline "Tous les labyrinthes générés sont valides" 
	else 
	  print_endline "Au moins un des labyrinthes générés n'est pas valide";exit 3
  | [ _; "print";"--pretty"; fichier ] ->
    if Sys.file_exists fichier then
		affiche_pretty (file_to_laby (open_in fichier))
    else
      print_endline "Le fichier n'existe pas"; exit 2
  | [ _; "print"; fichier ] ->
    if Sys.file_exists fichier then
      affiche_double_array (file_to_laby (open_in fichier)).grille
    else
      print_endline "Le fichier n'existe pas"; exit 2
  | [ _; "solve"; fichier ] ->
    if Sys.file_exists fichier then
      affiche_double_array (update_labyrinthe_avec_chemin (file_to_laby (open_in fichier)) (resoudre_labyrinthe (file_to_laby (open_in fichier)))).grille
    else
      print_endline "Le fichier n'existe pas"; exit 2
  | [ _; "solve";"--pretty"; fichier ] ->
    if Sys.file_exists fichier then
      affiche_pretty (update_labyrinthe_avec_chemin (file_to_laby (open_in fichier)) (resoudre_labyrinthe (file_to_laby (open_in fichier))))
    else
      print_endline "Le fichier n'existe pas"; exit 2
  | [ _; "random"; n; m ] -> affiche_double_array (generer_labyrinthe_aleatoire (int_of_string n) (int_of_string m) (abs (Random.bits ()))).grille (*On utilise Random.bits() car nous n'arrivons  pas à faire fonctionner Random.int en l'apellant avec max_int*)
  | [ _; "random"; n; m; r ] -> affiche_double_array (generer_labyrinthe_aleatoire (int_of_string n) (int_of_string m) (int_of_string r)).grille
  | _ -> print_endline "La commande n'existe pas, ou alors le nombre d'arguments donnés en entrée n'est pas bon";
         afficher_message_aide();
         exit 1
