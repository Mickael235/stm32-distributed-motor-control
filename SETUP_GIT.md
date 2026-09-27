# Mise en place Git / GitHub

## 1. Initialiser le dépôt local
```bash
git init
git branch -M main
git add .
git commit -m "chore: initialize project structure"
```

## 2. Créer un dépôt GitHub
Nom recommandé : `stm32-distributed-motor-control`

Ne cochez pas l'ajout automatique d'un README si vous utilisez déjà ce dossier.

## 3. Relier le dépôt local au dépôt GitHub
```bash
git remote add origin https://github.com/VOTRE-USER/stm32-distributed-motor-control.git
git push -u origin main
```

## 4. Créer les branches de travail
```bash
git checkout -b develop
git push -u origin develop

git checkout -b michael/can-fsm-supervision
git push -u origin michael/can-fsm-supervision

git checkout develop
git checkout -b nawel/motor-pid-freertos
git push -u origin nawel/motor-pid-freertos
```

Ensuite, travaillez par Issues et Pull Requests vers `develop`, puis mergez `develop` vers `main` lorsque le jalon est validé.
