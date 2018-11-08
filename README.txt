JALON 3

ZHENG PILLET

-Une fois la connexion établie, le client doit faire /nick Pseudo sinon le programme peut crasher

-Le serveur garde bien le socket fd et le pseudo. (implémentation de listes chaînées)
cependant la fonction deleteK(struct Client, int fd) ne marche pas...
On ne peut donc pas supprimer un élément avec le numéro fd de la liste chaînée.

-Puisqu'on ne parvient pas à supprimer un numéro fd de la liste chaînée,
les client ne doivent pas changer plus d'une fois de pseudo sous risque de faire crasher

-/who envoie au client fd la liste des utilisateurs connectés et déconnectés...

-On n'a pas réussi à afficher le port, et l'ip... commande /whois non faite

-le serveur garde son rôle répétitif
