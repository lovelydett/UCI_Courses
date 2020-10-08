/*
Author: Yuting Xie
Date:2020-10-8
Description: 253P HW1
*/ 

#include <stdio.h>
#include <string.h>

#define MAX_SIZE 1024
#define FILENAME "musicLibrary.txt"
#define LOGERR(a) printf("ERROR: \t"); printf(a); printf("\n");
#define LOGINFO(a) printf("INFO: \t"); printf(a); printf("\n");

inline void myFlush(){
	char ch;
	while((ch=getchar())!= '\n' && ch!=EOF);
}

struct Song {
	char title[MAX_SIZE];
	char artist[MAX_SIZE];
	int year_published;
	Song& operator=(Song& song){
		strcpy(artist, song.artist);
		strcpy(title, song.title);
		year_published = song.year_published;
		return *this;
	};
	bool operator<(Song& song){
		return strcmp(title, song.title) < 0 ;
	};
	bool operator>(Song& song){
		return strcmp(title, song.title) > 0 ;
	};
	bool operator==(Song& song){
		return strcmp(title, song.title) == 0 ;
	};
};


class MusicMgr
{
private:
	static MusicMgr mgr;//singleton instance
	Song songs[MAX_SIZE];
	int count = 0; //count of songs
	FILE* fp = nullptr;//file handle

public:
	static MusicMgr& getInstance() {
		return mgr;
	}
	void read_command();

private:
	MusicMgr(){}
	void print_song(int index);//display song info in one line
	void evaluate_command(char cmd);
	void print_MusicLibrary(bool toFile = false);
	//use when delete
	void crunch_up_from_index(int index); 
	//use when insert
	void crunch_down_from_index(int index); 
	int find_index_of_song_with_name(char *title);
	//find a proper place to insert into the array
	int find_proper_place_for_insert(Song& song);
	bool remove_song_from_MusicLibrary_by_name(char *title);
	bool add_song_to_MusicLibrary();
	//load the lib from a file
	void load_MusicLibrary(const char *fileName = FILENAME);
	//store the lib to a file
	void store_MusicLibrary(const char *fileName = FILENAME);
	FILE* open_file(const char *fileName = FILENAME, const char *mode = "r");

};

MusicMgr MusicMgr::mgr;//singleton instance

void MusicMgr::print_song(int index){
	if(index>=0 && index<count){
		printf("%d Title:%s, Artist:%s, Year Published:%d\n", index+1, songs[index].title, songs[index].artist, songs[index].year_published);
	}else{
		LOGERR("no such song with this index")
	}
	
}

void MusicMgr::read_command() {
	//load
	load_MusicLibrary();

	//loop to read a valid command
	char input;

	while(1){
		input = getchar();
		switch(input){

			case 'I':
			case 'P':
			case 'L':
			case 'D':
				evaluate_command(input);
				break;
			case 'Q':
				store_MusicLibrary();
				printf("Exiting, thanks for using\n");
				return;
			default:
				printf("Wrong input\n");

		}//switch

		myFlush();

	}//while
}

void MusicMgr::evaluate_command(char cmd){
	switch(cmd){

		case 'I':
			if(!add_song_to_MusicLibrary()){
				LOGERR("failed to add a song")
			}
			break;

		case 'P':
			print_MusicLibrary(false);
			break;

		case 'L':
		{
			char title[MAX_SIZE];
			printf("Title:");
			scanf("%s",title);
			int index = find_index_of_song_with_name(title);
			if(index>=0 && index<count){
				print_song(index);
			}else{
				LOGERR("No such song with this title")
			}
			
		}
			break;

		case 'D':
		{
			char title[MAX_SIZE];
			printf("Title:");
			scanf("%s",title);
			remove_song_from_MusicLibrary_by_name(title);
		}
			break;

		}//switch
}

void MusicMgr::print_MusicLibrary(bool toFile){
	for(int i=0; i<count; i++){
		print_song(i);
	}
}

void MusicMgr::crunch_up_from_index(int index){
	for (int i=index; i<count-1 ;i++){
		songs[i] = songs[i+1];
	}
}

void MusicMgr::crunch_down_from_index(int index){
	for (int i=count;i>index; i--){
		songs[i] = songs[i-1];
	}
}

int MusicMgr::find_index_of_song_with_name(char *title){
	
	//constuct a tmp song, used for compare
	Song tmpSong;
	strcpy(tmpSong.title,title);
	
	//binary search
	int left = 0, right = count-1, mid;
	while(left<=right){
		mid = (right+left)/2;
		if(songs[mid] == tmpSong){
			return mid;
		}else if(songs[mid]>tmpSong){
			right = mid-1;
		}else {//if(songs[mid]<tmpSong)
			left = mid+1;
		}
	}

	//query failed
	return -1;
}

int MusicMgr::find_proper_place_for_insert(Song& song){
	//song has been tested not in songs

	//edge cases
	if (0 == count) return 0;
	if (songs[0]>song) return 0;
	if (songs[count-1]<song) return count;

	//binary search
	int left = 0, right = count-1, mid;
	while(left<=right){
		mid = (left+right)/2;
		if (songs[mid]<song){
			left=mid+1;
		}else{ //if(songs[mid]>song)
			right = mid-1;
		}
	}

	//return songs[mid]>song ? mid-1 : mid+1;
	//not mid-1 but mid, cuz mid will be crunch back
	return songs[mid]>song ? mid : mid+1;
}

bool MusicMgr::remove_song_from_MusicLibrary_by_name(char *title){
	int index = find_index_of_song_with_name(title);
	if(index>=0 && index<count){
		crunch_up_from_index(index);
		count--;
		//no need to clean, use count for maintanance
	}else{
		LOGERR("Unable to delete, no such song with this title")
	}
}

bool MusicMgr::add_song_to_MusicLibrary(){

	if (MAX_SIZE==count) {
		LOGERR("max capacity has been reached, unable to insert")
		return false;
	}

	Song song;
	printf("Title:");
	scanf("%s",song.title);

	if(find_index_of_song_with_name(song.title)!=-1){
		//test if the title already exists
		LOGERR("Song with this title alreay exists")
		return false;
	}

	printf("Artist:");
	scanf("%s",song.artist);
	printf("Year Published:");
	scanf("%d",&song.year_published);

	//find a proper place
	int index = find_proper_place_for_insert(song);

	//crunch down
	if(index<count) 
		crunch_down_from_index(index);

	//finish insert
	songs[index] = song;
	count++;

	return true;

}

FILE* MusicMgr::open_file(const char *fileName, const char *mode){
	if(fp){
		LOGINFO("closing file before open")
		fclose(fp);
		fp = nullptr;
	}
	if(nullptr == (fp = fopen(fileName,mode))){
		LOGERR("cannot open file")
	}
	return fp;
}

void MusicMgr::load_MusicLibrary(const char *fileName){
	fp = open_file(FILENAME, "r");
	if (!fp){
		LOGERR("unable to open file for load, nothing loaded")
		return;
	}
	//1. read total num of songs
	int num;
	fscanf(fp,"%d",&num);
	if (num<0 || num>MAX_SIZE){
		LOGERR("invalid num of songs in this lib nothing loaded")
		return;
	}

	//2. read title/artist/yop in order
	for (int i=0; i<num; i++){
		fscanf(fp,"%s",songs[i].title);
		fscanf(fp,"%s",songs[i].artist);
		fscanf(fp,"%d",&songs[i].year_published);
	}
	count = num;
}

void MusicMgr::store_MusicLibrary(const char *fileName){
	fp = open_file(FILENAME,"w");
	if (!fp){
		LOGERR("unable to open file for store")
		return;
	}

	//1. write the total num of songs to file
	fprintf(fp,"%d\n",count);

	//2. write title/artist/yop in order
	for (int i=0; i<count; i++){
		fprintf(fp, "%s\n", songs[i].title);
		fprintf(fp, "%s\n", songs[i].artist);
		fprintf(fp, "%d\n", songs[i].year_published);
	}
}



int main(){
	MusicMgr& mgr = MusicMgr::getInstance();
	mgr.read_command();
	return 0;
}