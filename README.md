# Ensolement
Petit logiciel pour gérer l'ensolement des champs.<br>
Ceci est une première version très bête, une fois toutes les fonctionnalité présentes une passe sera faite pour changer l'algorithme.<br>
<br>
Les parcelles ainsi que les culture sont a renseigner via un fichier de configuration.<br>
<br>
Pour compiler le programme en prod :<br>
make all<br>
Puis pour le lancer :<br>
bin/ensolement "fichier de configuration"<br>
<br>
Pour compiler le programme avec les option de debug :<br>
make debug<br>
Puis pour le lancer :<br>
bin/ensolement "fichier de configuration"<br>
<br>
Pour compiler le programme et ces tests :<br>
make tests<br>
Puis pour le lancer :<br>
bin/test<br>