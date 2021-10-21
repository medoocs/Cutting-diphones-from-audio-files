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

static float buff[999999];
int ct = 0;

void razdvoji(char *audio){
  
  SF_INFO audioInfo;
  SNDFILE *wav = sf_open(audio, SFM_READ, &audioInfo);
  float *data = malloc(audioInfo.frames * sizeof *data);
  sf_readf_float(wav, data, audioInfo.frames);
  sf_close(wav);
  
  for(int i = 0; i < audioInfo.frames + 1; ++i){
    buff[ct] = data[i];
    ct++;
  }
  free(data);
}

void append(char* s, char c){
  int len = strlen(s);
  s[len] = c;
  s[len+1] = '\0';
}

int main(int argc, char **argv){
  char izlazAudio[MAX], tts[MAX], file[MAX];
  static char new[MAX][100] = {0};
  SF_INFO info;
  

  chdir("Difoni");
  struct dirent *de; 

  DIR *dr = opendir("."); 
    
  if(dr == NULL){ 
    printf("Ne postoji ta datoteka.\n"); 
    return 0; 
  }

  int br = 0, flag = 0;
  printf("Unesi tts: ");
  fgets(tts, MAX, stdin);
  if((strlen(tts) > 0) && (tts[strlen(tts) - 1] == '\n')) tts[strlen(tts) - 1] = '\0';
  for(int i = 0; i <= strlen(tts); ++i){
    if(i == 0){
      strcat(new[br], "sil");

      if(tts[i] == 'S'){
        strcat(new[br], "sh");
        br++;
      }else if(tts[i] == 'C'){
        strcat(new[br], "ch");
        br++;
      }else if(tts[i] == '1'){
        strcat(new[br], "cc");
        br++;
      }else if(tts[i] == 'D'){
        strcat(new[br], "dz");
        br++;
      }else if(tts[i] == 'Z'){
        strcat(new[br], "zh");
        br++;
      }else if(tts[i] == 'l' && tts[i+1] == 'j'){
        strcat(new[br], "lj");
        i++;
        br++;
      }else if(tts[i] == 'n' && tts[i+1] == 'j'){
        strcat(new[br], "nj");
        i++;
        br++;
      }else if(tts[i] == 'd' && tts[i+1] == 'Z'){
        strcat(new[br], "dj");
        i++;
        br++;
      }else{
        append(new[br], tts[i]);
        br++;
      }
      
    }else if(i == strlen(tts)){
      if(tts[i-1] == 'S'){
        strcat(new[br], "sh");
      }else if(tts[i-1] == 'C'){
        strcat(new[br], "ch");
      }else if(tts[i-1] == '1'){
        strcat(new[br], "cc");
      }else if(tts[i-1] == 'D'){
        strcat(new[br], "dz");
      }else if(tts[i-1] == 'Z'){
        strcat(new[br], "zh");
      }else if((tts[i-1] == 'l' && tts[i] == 'j') || (tts[i-1] == 'j' && tts[i-2] == 'l')){
        strcat(new[br], "lj");
        i++;
      }else if((tts[i-1] == 'n' && tts[i] == 'j') || (tts[i-1] == 'j' && tts[i-2] == 'n')){
        strcat(new[br], "nj");
        i++;
      }else if((tts[i-1] == 'd' && tts[i] == 'Z') || (tts[i-1] == 'Z' && tts[i-2] == 'd')){
        strcat(new[br], "dj");
        i++;
      }else{
        append(new[br], tts[strlen(tts) - 1]);
      }
      strcat(new[br], "sil");
      br++;
    }else{
      if(tts[i] == ' '){
        strcat(new[br], "silsil");
        br++;
      }else{
        if(tts[i-1] == ' '){
          strcat(new[br], "sil");

          if(tts[i] == 'S'){
            strcat(new[br], "sh");
            br++;
          }else if(tts[i] == 'C'){
            strcat(new[br], "ch");
            br++;
          }else if(tts[i] == '1'){
            strcat(new[br], "cc");
            br++;
          }else if(tts[i] == 'D'){
            strcat(new[br], "dz");
            br++;
          }else if(tts[i] == 'Z'){
            strcat(new[br], "zh");
            br++;
          }else if((tts[i] == 'l' && tts[i+1] == 'j')){
            strcat(new[br], "lj");
            i++;
            br++;
          }else if((tts[i] == 'n' && tts[i+1] == 'j')){
            strcat(new[br], "nj");
            i++;
            br++;
          }else if((tts[i] == 'd' && tts[i+1] == 'Z')){
            strcat(new[br], "dj");
            i++;
            br++;
          }else{
            append(new[br], tts[i]);
            br++;
          }
          
        }else{
          if(tts[i-1] == 'S'){
            strcat(new[br], "sh");
          }else if(tts[i-1] == 'C'){
            strcat(new[br], "ch");
          }else if(tts[i-1] == '1'){
            strcat(new[br], "cc");
          }else if(tts[i-1] == 'D'){
            strcat(new[br], "dz");
          }else if(tts[i-1] == 'Z'){
            strcat(new[br], "zh");
          }else if((tts[i-1] == 'j' && tts[i-2] == 'l')){
            strcat(new[br], "lj");
          }else if((tts[i-1] == 'j' && tts[i-2] == 'n')){
            strcat(new[br], "nj");
          }else if(tts[i-1] == 'Z' && tts[i-2] == 'd'){
            strcat(new[br], "dj");
          }else{
            append(new[br], tts[i-1]);
          }
       
          if(tts[i] == 'S'){
            strcat(new[br], "sh");
            br++;
          }else if(tts[i] == 'C'){
            strcat(new[br], "ch");
            br++;
          }else if(tts[i] == '1'){
            strcat(new[br], "cc");
            br++;
          }else if(tts[i] == 'D'){
            strcat(new[br], "dz");
            br++;
          }else if(tts[i] == 'Z'){
            strcat(new[br], "zh");
            br++;
          }else if(tts[i] == 'l' && tts[i+1] == 'j'){
            strcat(new[br], "lj");
            i++;
            br++;
          }else if(tts[i] == 'n' && tts[i+1] == 'j'){
            strcat(new[br], "nj");
            i++;
            br++;
          }else if(tts[i] == 'd' && tts[i+1] == 'Z'){
            strcat(new[br], "dj");
            i++;
            br++;
          }else{
            append(new[br], tts[i]);
            br++;
          }  
          
        }

        if(tts[i+1] == ' '){
          if(tts[i] == 'S'){
            strcat(new[br], "sh");
          }else if(tts[i] == 'C'){
            strcat(new[br], "ch");
          }else if(tts[i] == '1'){
            strcat(new[br], "cc");
          }else if(tts[i] == 'D'){
            strcat(new[br], "dz");
          }else if(tts[i] == 'Z'){
            strcat(new[br], "zh");
          }else if(tts[i] == 'j' && tts[i-1] == 'l'){
            strcat(new[br], "lj");
          }else if(tts[i] == 'j' && tts[i-1] == 'n'){
            strcat(new[br], "nj");
          }else if(tts[i] == 'Z' && tts[i-1] == 'd'){
            strcat(new[br], "dj");
          }else{
            append(new[br], tts[i]);
          }
          
          strcat(new[br], "sil");
          br++;
        }
        
      }
      
    }
  }
  for(int j = 0; j < br; ++j){
    while((de = readdir(dr)) != NULL){
      if(!strcmp (de->d_name, ".")) continue;
      if(!strcmp (de->d_name, "..")) continue;
      if(de->d_name[strlen(de->d_name) - 1] == 'v'){
        for(int g = 0; g < strlen(de->d_name); ++g){
          if(de->d_name[g] == '.') break;
          file[g] = de->d_name[g];
        }
      
        if(!strcmp(file, new[j])){
          flag++;
          if(flag == 1){
            SNDFILE *infoWav = sf_open(de->d_name, SFM_READ, &info);
            sf_close(infoWav);
          }
          razdvoji(de->d_name);
          rewinddir(dr);
          break;
        }
        memset(file, 0, sizeof file);
      }  

    }
  }
  closedir(dr);
  chdir("..");
  if(flag != br){
    printf("Baza nije potpuna za ovaj TTS.\n");
    return 0;
  }
  sprintf(izlazAudio, "%s.wav", tts);
  SNDFILE *izlaz = sf_open(izlazAudio, SFM_WRITE, &info);
  sf_writef_float(izlaz, buff, ct - 1);
  sf_close(izlaz);
  printf("Vas tekst je pretvoren u .wav datoteku.\n");
  return 0;
}