#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <time.h>

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

void current_time(char *buffer){
    time_t t;
    time(&t);
    struct tm*info=localtime(&t);
    strftime(buffer,30,"%Y-%m-%d %H:%M:%S",info);
}
char currentBridge[20]="BridgeA";

void pulsestatus(const char *msg){
for(int i=0;i<10;i++){
    printf("%s%s%s\r",RED,msg,RESET);
    fflush(stdout);
    Sleep(300);

    printf("%*s\r",(int)strlen(msg),"");
    fflush(stdout);
    Sleep(300);
}
    printf("%s%s%s\n",RED,msg,RESET);
    
}

void loading() {
    printf("\n");
    printf("%sLoading%s",YELLOW,RESET);
    fflush(stdout);

    for (int i = 0; i < 7; ++i) {
        Sleep(300);
        printf(".");
        fflush(stdout);
    }
    printf("\n\n");
}

void selectBridge() {
    
    int option;

    printf("\n%sSelect Bridge:%s\n",CYAN,RESET);
    printf("%s1. Bridge A%s\n",BLUE,RESET);
    printf("%s2. Bridge B%s\n",YELLOW,RESET);
    printf("%s3. Bridge C%s\n",GREEN,RESET);

    printf("Enter choice: ");
    scanf("%d", &option);

    switch(option) {
        case 1:
            strcpy(currentBridge, "bridgeA");
            break;
        case 2:
            strcpy(currentBridge, "bridgeB");
            break;
        case 3:
            strcpy(currentBridge, "bridgeC");
            break;
        default:
            printf("Invalid choice! Defaulting to Bridge A.\n");
            strcpy(currentBridge, "bridgeA");
    }
    loading();
    printf("Bridge selected: %s\n", currentBridge);
}

void enter_water_level() {
 

    int level;
    char filename[30], dangerFile[30], timeStr[30];

    
    sprintf(filename, "%s.txt", currentBridge);
    sprintf(dangerFile, "%s_danger.txt", currentBridge);

    printf("\n%sEnter water level (cm): %s",BLUE,RESET);
    scanf("%d", &level);
    
    current_time(timeStr);
    loading();
    
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("%sError opening file!%s\n",RED,RESET);
        return;
    }

    fprintf(fp, "%d  %s\n", level, timeStr);
    fclose(fp);

    printf("%sReading saved: %d cm at %s%s\n",GREEN,level,timeStr,RESET);

    
    if (level > 30) {
        printf("%s⚠️  WARNING! Water above safe limit.\n",RED,RESET);
        pulsestatus("Danger!");
        FILE *df = fopen(dangerFile, "a");

        if (df == NULL) {
        printf("%sError opening danger file%s",RED,RESET);
        }
        else {
        fprintf(df,"%d  %s\n",level, timeStr);
        fclose(df);
        }
    }

    else {
        printf("✔ Water level is safe.\n",GREEN,RESET);
    }
}

void view_previousreadings() {
    char filename[30];
    sprintf(filename, "%s.txt", currentBridge);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("\n%s❌ No readings found for this bridge.%s\n",YELLOW,RESET);
        return;
    }

    printf("\n%s📜 Previous Water Level Readings (%s):%S\n", CYAN,currentBridge,RESET);
    printf("-------------------------------------------\n");

    int level;
    char timeStr[30];

    while (fscanf(fp, "%d  %s", &level, timeStr) != EOF) {
        printf("%sWater Level: %d cm   Time: %s%s\n",BLUE, level, timeStr,RESET);
    }

    fclose(fp);
}

void check_dangerstatus() {

    
    char filename[30];
    sprintf(filename, "%s.txt", currentBridge);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("\n%s❌ No readings found for this bridge.%s\n",YELLOW,RESET);
        return;
    }

    int level;
    char timeStr[30];
    int lastLevel = -1;
    char lastTime[30];

    
    while (fscanf(fp, "%d  %s", &level, timeStr) != EOF) {
        lastLevel = level;
        strcpy(lastTime, timeStr);
    }

    fclose(fp);

    if (lastLevel == -1) {
        printf("\n%s❌ No valid readings found.\n",YELLOW,RESET);
        return;
    }

    printf("\n%s📌 Latest Reading for %s:%s\n",BLUE, currentBridge,RESET);
    printf("-------------------------------------------\n");
    printf("%sWater Level : %d cm%S\n",CYAN ,lastLevel,RESET);
    printf("Time        : %s\n",CYAN, lastTime,RESET);

    if (lastLevel > 30) {
        printf("%s⚠️  STATUS: DANGER (Above Safe Limit)\n",RED,RESET);
        pulsestatus("DANGER!");

    } else {
        printf("%s✔ STATUS: SAFE\n",GREEN,RESET);
    }
}

void viewDangerLog() {

    char filename[40];
    sprintf(filename, "%s_danger.txt", currentBridge);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("\n%s❌ No danger logs found for this bridge.%s\n", YELLOW, RESET);
        return;
    }

    printf("\n%s🚨 Danger Log for %s:%s\n", RED, currentBridge, RESET);
    printf("-------------------------------------------\n");

    int level;
    char timeStr[30];
    int found = 0;

    while (fscanf(fp, "%d %s", &level, timeStr) == 2) {
        found = 1;
        printf("%sLevel: %d cm   Time: %s%s\n", YELLOW, level, timeStr, RESET);
    }

    if (!found) {
        printf("\n%s✔ No danger events recorded.%s\n", GREEN, RESET);
    }

    fclose(fp);
}


void viewStatistics() {
    

    char filename[40];
    sprintf(filename, "%s.txt", currentBridge);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("\n%s❌ No data available for statistics.%s\n",RED,RESET);
        return;
    }

    int level;
    char timeStr[30];
    
    int max = -9999;
    int min = 9999;
    int sum = 0;
    int count = 0;

    while (fscanf(fp, "%d  %s", &level, timeStr) != EOF) {
        if (level > max) max = level;
        if (level < min) min = level;
        sum += level;
        count++;
    }

    fclose(fp);

    if (count == 0) {
        printf("\n%s❌ No readings found for statistics.%s\n",RED,RESET);
        return;
    }

    float avg = (float)sum / count;

    printf("\n📊 %sWater Level Statistics for %s%s\n",BLUE,currentBridge,RESET);
    printf("-------------------------------------------\n");
    printf("%sTotal Readings : %d%s\n",YELLOW,count);
    printf("%sMaximum Level  : %d cm%s\n",RED ,max);
    printf("%sMinimum Level  : %d cm%s\n",BLUE, min);
    printf("%sAverage Level  : %.2f cm%s\n",GREEN,avg);

}

void aciigraph(){
    
    char filename[30];
    sprintf(filename,"%s.txt",currentBridge);

    FILE *fp = fopen(filename,"r");
    if(fp==NULL){
        printf("\n%s❌ No data available for graph.%s\n",RED,RESET);
    }

    int level;
    char timestr[30];
    int lastlevel=-1;

    while(fscanf(fp,"%d %s",&level,timestr)!=EOF){
        lastlevel =level;
    }
    fclose(fp);
    if(lastlevel==-1){
        printf("\n%s❌ No readings found.%s\n",RED,RESET);
        return;
    }
    printf("\n%s📊 ASCII Water Level Graph (%s)%s\n",GREEN,currentBridge,RESET);
    printf("-------------------------------------------\n");
    printf("%sLatest Level : %d cm%s\n\n",BLUE,level,RESET);

    int bars=lastlevel/2;

    printf("[");
    for(int i=0;i<bars;i++){
        printf("█");
    }
    for(int i=bars;i<50;i++){
        printf("-");
    }
    printf("] (%d cm)\n",lastlevel);

    if(lastlevel>30){
        printf("\n%s⚠️  STATUS: DANGER!%s\n",RED,RESET);
    } 
    else {
        printf("\n%s✔ STATUS: SAFE%s\n",GREEN,RESET);
    }
}
void enter_to_cont(){
    printf("\n%sPress Enter to Continue....%s",BLUE,RESET);
    getchar();
    getchar();
}

int main() {
    
    int choice;
    char buffer[30];
    char msg[30]="Danger!!";

    while(1){
        printf("\n%sRailway Bridge Water level Management System%s\n",BLUE,RESET);
        printf("\t\n%s1.Select Bridge(ex-bridge1)%s\n",GREEN,RESET); 
        printf("\t\n%s2.Enter New Water Level(in cm)%s\n",GREEN,RESET);
        printf("\t\n%s3.View Previous Readings%s\n",CYAN,RESET);
        printf("\t\n%s4.View Statistics%s\n",CYAN,RESET);
        printf("\t\n%s5.Check Danger Status%s\n",RED,RESET);
        printf("\t\n%s6.View Danger Log%s\n",RED,RESET);
        printf("\t\n%s7.ASCII Graph%s\n",CYAN,RESET);
        printf("\t\n%s8.Exit%s\n",BLUE,RESET);

        Sleep(2000);

        printf("\t\n%sEnter your choice: %s",YELLOW,RESET);
        scanf( "%d" , &choice);

        loading();
    

        if(choice==1){
        selectBridge();
        enter_to_cont();
        
        }
        else if(choice==2){
        enter_water_level();
            enter_to_cont();
        }
        else if(choice==3){
        view_previousreadings();
        enter_to_cont();
        }
        else if(choice==5){
        check_dangerstatus();
        enter_to_cont();
        }
        else if(choice==6){
        viewDangerLog();
        enter_to_cont();
        }
        else if(choice==4){
        viewStatistics();
        enter_to_cont();
        }
        else if(choice==7){
        aciigraph();
        enter_to_cont();
        }
        else if(choice ==8){
        printf("Exiting the program.......");
        enter_to_cont();
        }
    }

    return 0;

}