#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm> // for random_shuffle
#define MAX_SIZE 1024

int main(){
    //0. open file
    FILE* fp = fopen("roster.txt","r");
    if(!fp){
        return 0;
    }

    //1. read until "Student#" and also get the total number of students
    unsigned numStu = 0;
    char buffer[MAX_SIZE];

    while(1){
        if(0 > fscanf(fp, "%s",buffer))
            return 0;
        if (0==strcmp(buffer,"Enrollment:")){
            fscanf(fp, "%d",&numStu);
        }
        else if (0==strcmp(buffer,"Student#"))
            break;
    }

    //2. drop "Name	Email	Major	Lvl	Opt	Notes" headers
    fgets(buffer, MAX_SIZE, fp);

    //3. loop to read names
    unsigned id;
    std::unordered_set<std::string> names;

    auto split = [](char* s){
        for (int i = 0; i<strlen(s); i++){
            if('\t' == s[i] || '\n' == s[i]){
                s[i] = '\0';
                return;
            }
        }
    };

    for(int i=0; i<numStu; i++){
        //get id
        if(0 > fscanf(fp, "%d",&id)){
            printf("mark A\n");
            break; //finish, time to break
        }

        //get remaining content
        fgetc(fp); //drop '\t'
        fgets(buffer, MAX_SIZE, fp);

        //change 1st '\t' to '\0' to drop remaining content
        split(buffer);
        
        //store it
        names.insert(std::string(buffer));

    }

    fclose(fp);

    //4. get group size and absent students
    unsigned groupSize;
    printf("group size:\n\t");
    scanf("%d",&groupSize);
    getchar();//drop '\n'

    printf("absent students:\n");
    while(1){
        printf("\t");

        memset(buffer,0,MAX_SIZE);
        fgets(buffer, MAX_SIZE, stdin);
        split(buffer);
        if (0==strlen(buffer))
            break;
        
        //remove from names
        auto name = std::string(buffer);
        if( names.end()!= names.find(name)){
            names.erase(name);
        }
    }

    //5. calculate group num and residual num
    unsigned numGroup = names.size()/groupSize,
        numResi = names.size() % groupSize;

    //6. randomly assign these students to different "groups"
    // the [0-groupSize-1] of vector is group1, [groupSize-2*groupSize-1] is group2 ...
    // and the last numResi students are in order assigned to group1, group2, group3 ...
    numStu = names.size();
    std::vector<std::string> groupedNames;
    for (auto name : names){
        groupedNames.push_back(name);
    }

    std::random_shuffle(groupedNames.begin(), groupedNames.end());
       
    //7. show results
    printf("groups:\n");
    for(int i=0;i<numGroup;i++){
        printf("\n\t%d.\n",i+1);

        //normal students
        for( int j=0; j<groupSize; j++){
            printf("\t%s\n",groupedNames[i*groupSize+j].c_str());
        }

        //residual students
        if (i<numResi){
            printf("\t%s\n",groupedNames[groupSize*numGroup+i].c_str());
        }
        
    }

    return 0;
}