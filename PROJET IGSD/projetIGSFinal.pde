PShape terrain;
PShader levelLinesShader;
float camX = 0;
float camY = 0;
float camZ = -175;
float angle=0.03;
float scaleEolienne = 0.02;
PVector dir = new PVector(100,0,0);

PImage[] bgFrames;
int frameIndex = 0;
int totalFrames = 40; 
boolean descend=false;
boolean eoliennes = false;
boolean poteaux = false;
boolean monte=false;

PVector[] positionsDesPoteaux = new PVector[25];
boolean moveLeft, moveRight, moveForward,moveBackward;
PShape poteau; 
PShape transfo;
int couleurBois;
float echelle=0.25; 

public void setup() {
  size(1000, 1000, P3D);
  //terrain = loadShape("SIMPLIFY/simplify.obj");
  terrain = loadShape("HYPERSIMPLE/hypersimple.obj");
  bgFrames = new PImage[totalFrames];
  for (int i = 0; i < totalFrames; i++) {
    bgFrames[i] = loadImage("background/image_" + nf(i, 3) + ".jpg"); // Assurez-vous que vos fichiers sont nommés correctement
  }

  //terrain = loadShape("SIMPLIFY/simplify.obj");
  levelLinesShader = loadShader("MyFragmentShader.glsl","MyVertexShader.glsl");
  

  
  couleurBois = color(156, 102, 31);
  poteau = createPoteau(); 
  transfo=dessinTransformateur();
  creePoteauxElectriques(20, 100, 40, -115, 25);
}

public void draw() {
  dir.normalize();
  PVector right = new PVector(dir.y, -dir.x, 0);
  right.normalize();
  if (moveLeft) {
        camX += right.x *1.5;
        camY += right.y *1.5 ;
    }
    if (moveRight) {
        camX -= right.x *1.5 ;
        camY -= right.y *1.5;
    }
    if (moveForward) {
        camX += dir.x *1.5;
        camY += dir.y *1.5;
    }
    if (moveBackward) {
        camX -= dir.x *1.5;
        camY -= dir.y *1.5;
    }
    if(monte){
      camZ+=0.5;
    }
    if(descend){
      camZ-=0.5;
   }
   
  
  background(bgFrames[frameIndex]);
  frameIndex = (frameIndex + 1) % totalFrames;

  camera(camX, camY, camZ, camX+dir.x, camY+dir.y, camZ+dir.z, 0, 0,-1);
  float fov = PI/3.0;
  float cameraZ = (height/2.0) / tan(fov/2.0);
  perspective(fov, float(width)/float(height), 0.01, cameraZ*10.0);
  
  shader(levelLinesShader);
  
  shape(terrain);
  resetShader();
  drawAxes();
  
  PVector posEolienne1 = new PVector(positionsDesPoteaux[0].x, positionsDesPoteaux[0].y+20, getTerrainAltitude(terrain, positionsDesPoteaux[0].x, positionsDesPoteaux[0].y+20)+5);
  PVector posEolienne2 = new PVector(positionsDesPoteaux[positionsDesPoteaux.length-1].x, positionsDesPoteaux[positionsDesPoteaux.length-1].y-20, getTerrainAltitude(terrain, positionsDesPoteaux[positionsDesPoteaux.length-1].x, positionsDesPoteaux[positionsDesPoteaux.length-1].y-20)+5);
  if(poteaux){
    drawPoteauElectrique(positionsDesPoteaux[0].x, positionsDesPoteaux[0].y);
    for (int i = 1; i < 25; i++) {
        drawPoteauElectrique(positionsDesPoteaux[i].x, positionsDesPoteaux[i].y);
        strokeWeight(2);
        stroke(0);
        float x0 = positionsDesPoteaux[i - 1].x;
        float y0 = positionsDesPoteaux[i - 1].y;
        float z0 = getTerrainAltitude(terrain, x0, y0) + 24 * echelle;
        float x1 = positionsDesPoteaux[i].x;
        float y1 = positionsDesPoteaux[i].y;
        float z1 = getTerrainAltitude(terrain, x1, y1) + 24 * echelle;
   
        float previousX = x0;
        float previousY = y0;
        float previousZ = z0;
    
        for (int j = 1; j <= 50; j++) {
            float t = j / 50.0; // Fraction du chemin parcouru
    
            float lx = x0 + t * (x1 - x0);
            float ly = y0 + t * (y1 - y0);
            
            float amplitude = 10 * echelle; // Hauteur de la courbe au milieu entre les poteaux
            float dz = amplitude * sin(-PI * t); // Sinusoidale
            
            float lz = z0 + t * (z1 - z0) + dz;
            
            // Dessiner la ligne du point précédent au point actuel
            line(previousX-1, previousY, previousZ, lx-1, ly, lz);
            line(previousX+1, previousY, previousZ, lx+1, ly, lz);
            
            line(previousX-1, previousY, previousZ+0.75, lx-1, ly, lz+0.75);
            line(previousX+1, previousY, previousZ+0.75, lx+1, ly, lz+0.75);
            
    
            // Mémoriser les coordonnées actuelles pour la prochaine itération
            previousX = lx;
            previousY = ly;
            previousZ = lz;
        }
    }
    
    if(eoliennes){
      PVector posTransfo1 = new PVector(positionsDesPoteaux[0].x, positionsDesPoteaux[0].y+10, getTerrainAltitude(terrain, positionsDesPoteaux[0].x, positionsDesPoteaux[0].y+10)+2);
      drawTransfo(posTransfo1);
      PVector posTransfo2 = new PVector(positionsDesPoteaux[positionsDesPoteaux.length-1].x, positionsDesPoteaux[positionsDesPoteaux.length-1].y-10, getTerrainAltitude(terrain, positionsDesPoteaux[positionsDesPoteaux.length-1].x, positionsDesPoteaux[positionsDesPoteaux.length-1].y-10));
      drawTransfo(posTransfo2);
      connectTransfoToPole(posTransfo1, positionsDesPoteaux[0]);
      connectTransfoToPole(posTransfo2, positionsDesPoteaux[positionsDesPoteaux.length-1]);
      connectTransfoToPole(posEolienne1,posTransfo1);
      connectTransfoToPole(posEolienne2,posTransfo2);
    }
  }
  if(eoliennes){
    
    drawEolienne(posEolienne1, scaleEolienne); 
    
    // Placer une éolienne au dernier poteau
    
    drawEolienne(posEolienne2, scaleEolienne); 
  }


}


void drawAxes() {
  pushMatrix();
  translate(0,0,-175); 
  stroke(255, 0, 0);
  line(0, 0, 0, 10, 0, 0);
  stroke(0, 255, 0);
  line(0, 0, 0, 0, 10, 0);
  stroke(0, 0, 255);
  line(0, 0, 0, 0, 0, 10);
  popMatrix();
}

public void keyPressed() {
    switch (key) {
        case 'q':
        case 'Q':
            moveLeft = true;
            break;
        case 'd':
        case 'D':
            moveRight = true;
            break;
        case 'z':
        case 'Z':
            moveForward = true;
            break;
        case 's':
        case 'S':
            moveBackward = true;
            break;
       case 'e':
       case 'E':
           eoliennes = !eoliennes;
           break;
       case 'p':
       case 'P':
           poteaux = !poteaux; 
           break;
       case ' ':
           monte=true;
           break;
       case CODED: // Pour les touches spéciales comme Majuscule
            if (keyCode == SHIFT) { // Vérifier si la touche est Majuscule
                descend=true; // Descendre la caméra
            }
            break;
    }
}

public void keyReleased() {
    switch (key) {
        case 'q':
        case 'Q':
            moveLeft = false;
            break;
        case 'd':
        case 'D':
            moveRight = false;
            break;
        case 'z':
        case 'Z':
            moveForward = false;
            break;
        case 's':
        case 'S':
            moveBackward = false;
            break;
        case ' ':
           monte=false;
           break;
        case CODED: // Pour les touches spéciales comme Majuscule
            if (keyCode == SHIFT) { // Vérifier si la touche est Majuscule
                descend=false; // Descendre la caméra
            }
            break;
    }
}


float PosX;
float PosY;

public void mousePressed() {
  PosX = mouseX;
  PosY = mouseY;
}


public void mouseWheel(MouseEvent event) {
  float e = event.getCount();
  float speed = 2; 
  camZ += -e * speed;
}


public void mouseDragged() {
  float diffX = mouseX-PosX;
  float diffY = mouseY-PosY;

  float rotateSpeed=0.005; 
  float angleY=diffX * rotateSpeed;
  dir.rotate(angleY);
  dir.normalize();
  float angleX = diffY*rotateSpeed;
  dir.rotate(angleX);
  dir.normalize();
  PosX=mouseX;
  PosY=mouseY;
}


PShape createPoteau() {
  PShape grp = createShape(GROUP);
  pushMatrix();
  fill(88,41,0);
  noStroke();
  PShape corps = createCylinder();
  corps.translate(0, 0, 1.5);
  grp.addChild(corps);
  
  
  
  PShape bras1 = createShape(BOX, 12*echelle, 2*echelle, 1*echelle);
  bras1.setFill(couleurBois);
  bras1.translate(0, 0, 12*echelle);
  grp.addChild(bras1);

  PShape bras2 = createShape(BOX, 12*echelle, 2*echelle, 1*echelle);
  bras2.setFill(couleurBois);
  bras2.translate(0, 0,15*echelle);
  grp.addChild(bras2);
  
  PShape bras3 = createShape(BOX, 12*echelle, 2*echelle, 1*echelle);
  bras3.setFill(couleurBois);
  bras3.translate(0, 0,18*echelle);
  grp.addChild(bras3);
  
  PShape diag1 = createShape(BOX,6*echelle,2*echelle,echelle);
  diag1.setFill(couleurBois);
  diag1.rotateY(PI/4);
  diag1.translate(-0.5,0,16*echelle);
  grp.addChild(diag1);
 
  PShape diag2 = createShape(BOX,6*echelle,2*echelle,echelle);
  diag2.setFill(couleurBois);
  diag2.rotateY(-PI/4);
  diag2.translate(0.5,0,16*echelle);
  grp.addChild(diag2);
  popMatrix();
  
  return grp;
} 



PShape createCylinder() {
  PShape corps = createShape(BOX, 0.5, 0.5, 8); 
  corps.setFill(couleurBois);
  return corps;
}

float getTerrainAltitude(PShape terrain, float x, float y) {
    float Z = 0;
    float min = Float.MAX_VALUE; 

    for (int i = 0; i < terrain.getChildCount(); i++) {
        PShape child = terrain.getChild(i);
        for (int j = 0; j < child.getVertexCount(); j++) {
            PVector vertex = child.getVertex(j);
            float distance = dist(x, y, vertex.x, vertex.y); 
            if (distance < min) {
                min = distance;
                Z = vertex.z; 
            }
        }
    }

    return Z;
}


void drawPoteauElectrique(float x, float y) {
    float z = getTerrainAltitude(terrain, x, y);
    pushMatrix();
    translate(x, y, z+8*echelle);
    shape(poteau);
    popMatrix();
}

void drawTransfo(PVector coord) {
    pushMatrix();
    translate(coord.x,coord.y,coord.z);
    shape(transfo);
    popMatrix();
}

void creePoteauxElectriques(float x1, float y1, float x2, float y2, int N) {
  float deltaX = (x2 - x1) / (N - 1); //espacement entre deux poteaux
  float deltaY = (y2 - y1) / (N - 1);
  
  for (int i = 0; i < N; i++) {
    float x = x1 + i * deltaX;
    float y = y1 + i * deltaY;
    positionsDesPoteaux[i]=new PVector(x,y);
    
  }
}
void cone(float r, float h) {
    int detail = 24; // Nombre de côtés du cône
    float angleStep = TWO_PI / detail;
    beginShape(TRIANGLE_FAN);
    vertex(0, -h, 0); // pointe du cône
    for (float a = 0; a <= TWO_PI + angleStep; a += angleStep) {
        float x = r * cos(a);
        float z = r * sin(a);
        vertex(x, 0, z); // base du cône
    }
    endShape(CLOSE);
}

void drawEolienne(PVector position, float scaleEolienne) {
  pushMatrix();
  translate(position.x, position.y, position.z);
  scale(scaleEolienne);
  rotateX(-PI / 2); 
  // Dessiner la base de l'éolienne
  fill(255);
  pushMatrix();
  translate(0, 150 ,0);
  box(20, 300, 20);
  popMatrix();
  
  // Dessiner le corps de l'éolienne
  fill(255);
  pushMatrix();
  translate(0, 0, 0);
  cylinder(10, 200);
  popMatrix();
  
  // Dessiner le nez de l'éolienne
  fill(200, 200, 200); // Choisissez une couleur grise pour le nez
  pushMatrix();
  translate(0, -176, 10); // Positionner le nez au bout du corps
  rotateX(-PI/2);
  cone(10, 20); // Dessiner le cône avec un rayon de 5 et une hauteur de 20
  popMatrix();
  
  // Dessiner les pales qui tournent
  pushMatrix();
  translate(0, -180, 10);
  rotateZ(angle * (frameCount *4));
  for (int i = 0; i < 3; i++) {
    pushMatrix();
    rotateZ(TWO_PI / 3 * i);
    drawBlade();
    popMatrix();
  }
  popMatrix();
  popMatrix();
}

void drawBlade() {
  // Utiliser des courbes de Bézier pour dessiner une pale
  fill(255);
  beginShape();
  vertex(0, 10);
  bezierVertex(-5, -2, -40, -20, 0, -200);
  bezierVertex(40, -20, 5, -2, 0, 0);
  endShape(CLOSE);
}

void cylinder(float r, float h) {
  // Dessiner un cylindre vertical
  int detail = 24; // Nombre de côtés du cylindre
  float angleStep = TWO_PI / detail;
  beginShape(QUAD_STRIP);
  for (float a = 0; a <= TWO_PI; a += angleStep) {
    float x = r * cos(a);
    float z = r * sin(a);
    vertex(x, 0, z);
    vertex(x, -h, z); 
  }
  endShape(CLOSE);
}

PShape dessinTransformateur() {
  PShape grp = createShape(GROUP);
  
  // Corps principal du transformateur
  PShape corps = createShape(BOX, 2.6, 1.3, 2.6);
  corps.setFill(color(100, 100, 100));
  grp.addChild(corps);

  // Réservoir cylindrique sur le côté
  PShape reservoir = createCylinder(0.4, 1.6);
  reservoir.setFill(color(150, 150, 150));
  reservoir.translate(1.3, 0, 0.8);
  grp.addChild(reservoir);

  // Isolateurs sur le dessus
  float isolatorSpacing = 0.66; 
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      PShape isolateur = createCylinder(0.2, 0.5); 
      isolateur.setFill(color(200, 200, 200));
      isolateur.translate(i * isolatorSpacing, j * isolatorSpacing, 1.3);
      grp.addChild(isolateur);
    }
  }

  // Connecteurs métalliques sur les isolateurs
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      PShape connecteur = createShape(BOX, 0.04, 0.04, 0.2);
      connecteur.setFill(color(0));
      connecteur.translate(i * isolatorSpacing, j * isolatorSpacing, 1.35);
      grp.addChild(connecteur);
    }
  }

  return grp;
}

PShape createCylinder(float r, float h) {
  PShape cyl = createShape();
  cyl.beginShape(TRIANGLE_STRIP);
  for (int i = 0; i < 360; i += 10) {
    float angle = radians(i);
    float x = r * cos(angle);
    float z = r * sin(angle);
    cyl.vertex(x, z, -h/2);
    cyl.vertex(x, z, h/2);
  }
  cyl.vertex(0, r, -h/2);
  cyl.vertex(0, r, h/2);
  cyl.endShape(CLOSE);
  
  return cyl;
}



void connectTransfoToPole(PVector transfoPos, PVector polePos) {
    strokeWeight(2);
    stroke(0);
    float zAdjust = 6 * echelle; 
    line(transfoPos.x, transfoPos.y, transfoPos.z, polePos.x, polePos.y, getTerrainAltitude(terrain, polePos.x, polePos.y) + zAdjust);
}
