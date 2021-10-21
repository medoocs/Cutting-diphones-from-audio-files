#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sndfile.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <ctype.h>
#define MAX 99999

int poljeCt[MAX] = {0}, brMaxD = -1, ctDif = 0, ctDif2 = 0, brFile = -1;
char maxDif[20], poljeDif[MAX][10] = {0};

int hash(char *difon, int n){
  int br = 0;
  for(int i = 0; i < n; ++i){
    br += br*53 + (int)difon[i];
    br %= 99999;
  }
  return br;
}

void spoji(char *labRijec1, int brL1, char *labRijec2, int brL2, char *difon){
  int ct = 0;
  for(int i  = 0; i < brL1; ++i){
    if(labRijec1[i] == ':') continue;
    if(labRijec1[i] == 'C'){
      difon[ct] = 'c';
      ct++;
      difon[ct] = 'h';
      ct++;
      continue;
    }
    if(labRijec1[i] == 'D' && labRijec1[i+1] == 'Z'){
      difon[ct] = 'd';
      ct++;
      difon[ct] = 'j';
      ct++;
      i++;
      continue;
    }
    if(labRijec1[i] == 'L'){
      difon[ct] = 'l';
      ct++;
      difon[ct] = 'j';
      ct++;
      continue;
    }
    if(labRijec1[i] == 'N'){
      difon[ct] = 'n';
      ct++;
      difon[ct] = 'j';
      ct++;
      continue;
    }
    if(labRijec1[i] == 'S'){
      difon[ct] = 's';
      ct++;
      difon[ct] = 'h';
      ct++;
      continue;
    }
    if(labRijec1[i] == 'Z'){
      difon[ct] = 'z';
      ct++;
      difon[ct] = 'h';
      ct++;
      continue;
    }
    difon[ct] = labRijec1[i];
    ct++;
  }

  difon[ct] = '-';
  ct++;

  for(int i  = 0; i < brL2; ++i){
    if(labRijec2[i] == ':') continue;
    if(labRijec2[i] == 'C'){
      difon[ct] = 'c';
      ct++;
      difon[ct] = 'h';
      ct++;
      continue;
    }
    if(labRijec2[i] == 'D' && labRijec2[i+1] == 'Z'){
      difon[ct] = 'd';
      ct++;
      difon[ct] = 'j';
      ct++;
      i++;
      continue;
    }
    if(labRijec2[i] == 'L'){
      difon[ct] = 'l';
      ct++;
      difon[ct] = 'j';
      ct++;
      continue;
    }
    if(labRijec2[i] == 'N'){
      difon[ct] = 'n';
      ct++;
      difon[ct] = 'j';
      ct++;
      continue;
    }
    if(labRijec2[i] == 'S'){
      difon[ct] = 's';
      ct++;
      difon[ct] = 'h';
      ct++;
      continue;
    }
    if(labRijec2[i] == 'Z'){
      difon[ct] = 'z';
      ct++;
      difon[ct] = 'h';
      ct++;
      continue;
    }
    difon[ct] = labRijec2[i];
    ct++;
  }
}

void nadiMax(int br, char *difon){
  if(poljeCt[br] > brMaxD){
    brMaxD = poljeCt[br];
    memset(maxDif, 0, sizeof maxDif);
    strcpy(maxDif, difon);
  }
}

void razdvoji(const char *audio, const char *lab){
  
  SF_INFO audioInfo;
  SNDFILE *wav = sf_open(audio, SFM_READ, &audioInfo);
  float *data = malloc(audioInfo.frames * sizeof *data);
  sf_readf_float(wav, data, audioInfo.frames);
  sf_close(wav);

  FILE *labFile = fopen(lab, "r");
  if(labFile == NULL) return;
  
  const float microSek = audioInfo.samplerate/1e7; 
	
  chdir("Difoni");

  int s, k, start, kraj;
  char difon[50] = {0}, labRijec1[20] = {0}, labRijec2[20] = {0}, izlazAudio[100];
  
  fscanf(labFile, "%d%d%s", &s, &k, labRijec1);
  if(feof(labFile)) return;
  start = (s + k) / 2;

  while (1){
    
    fscanf(labFile, "%d%d%s", &s, &k, labRijec2);
    if(feof(labFile)) break;
    kraj = (s+k)/2;
    
    spoji(labRijec1, strlen(labRijec1), labRijec2, strlen(labRijec2), difon);
    
    int br = hash(difon, strlen(difon));
    
    poljeCt[br]++;
    ctDif2++;

    if(poljeCt[br] == 1){
      ctDif++;
      strcpy(poljeDif[br], difon);
    } 
    
    nadiMax(br, difon);
    
    int ms = start*microSek, me = kraj*microSek;
    mkdir(difon);
    sprintf(izlazAudio, "%s/%s-%d.wav", difon, difon, poljeCt[br]);
    SNDFILE *izlaz = sf_open(izlazAudio, SFM_WRITE, &audioInfo);
    float buf[me-ms];

    for (int i = 0; i < me-ms; ++i){
   	  buf[i] = data[ms+i];
    }
    
    sf_writef_float(izlaz, buf, me-ms);
    sf_close(izlaz);
    
    start = kraj;
    memset(labRijec1, 0, sizeof labRijec1);
    strcpy(labRijec1, labRijec2);
    memset(labRijec2, 0, sizeof labRijec2);
    memset(difon, 0, sizeof difon);
  
  }

  free(data);
  fclose(labFile);
}

void ucitaj(const char *ime) {
  int n = strlen(ime) + 10;
  char *wav = malloc(n * sizeof *wav), *lab = malloc(n * sizeof *lab);

  strcpy(wav, ime); strcat(wav, ".wav");
  strcpy(lab, ime); strcat(lab, ".lab");

  razdvoji(wav, lab);
  brFile++;

  free(wav); 
  free(lab);
}

void sort() {
  for (int i = 0; i < MAX; i++) {
    for (int j = 1; j < MAX; j++) {
      if (poljeCt[j - 1] < poljeCt[j]) {
        int tmp = poljeCt[j - 1];
        poljeCt[j - 1] = poljeCt[j];
        poljeCt[j] = tmp;
        char tmp2[50];
        strcpy(tmp2, poljeDif[j-1]);
        strcpy(poljeDif[j-1], poljeDif[j]);
        strcpy(poljeDif[j], tmp2);
      }
    }
  }
}

int main(int argc, char **argv){

  time_t poc = time(NULL);

  chdir("Baza");

  FILE *txtFile = fopen("Histogram.txt", "a");
  if(txtFile == NULL) printf("Nije moguce napravit histogram.\n");

  mkdir("Difoni");
  struct dirent *de; 

  DIR *dr = opendir("."); 
  if(dr == NULL){ 
    printf("Nemogu otvoriti bazu.\n"); 
    return 0; 
  }

  while((de = readdir(dr)) != NULL){
    
    if(!strcmp (de->d_name, ".")) continue;
    if(!strcmp (de->d_name, "..")) continue;
    if(de->d_name[strlen(de->d_name) - 1] == 'v' && de->d_name[strlen(de->d_name) - 2] == 'a' && de->d_name[strlen(de->d_name) - 3] == 'w'){
      char file[200] = {0};
      for(int i = 0; i < strlen(de->d_name); ++i){
        if(de->d_name[i] == '.') break;
        file[i] = de->d_name[i];
      }
      ucitaj(file);
      chdir("..");
    }    
  }
  closedir(dr); 

  printf("Izrezano je %d difona od cega %d unikatnih, a najvise se pojavljuje difon: %s i to %d puta.\n", ctDif2, ctDif, maxDif, brMaxD);

  sort();

  for(int i = 0; i < MAX; ++i){
    if(poljeCt[i] != 0 && strcmp(poljeDif[i], "uzdah-bukaf-r") != 0) fprintf(txtFile,"%s %d\n", poljeDif[i], poljeCt[i]);
  }

  time_t end = time(NULL);

  printf("Trebalo je %d sekundi da se obradi %d .wav i .lab datoteka.\n", (end - poc), brFile);
  printf("U datoteci Histogram.txt spremljeni su svi primjerci difona i njihov broj ponavljanja.\n");

  return 0;
}