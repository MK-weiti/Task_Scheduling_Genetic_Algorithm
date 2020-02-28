CC=g++
KONF=-std=c++11
FLAG=-c
MAIN=genetic

all: $(MAIN)

debug: KONF+=-g -DDEBUG
debug: $(MAIN)


$(MAIN): task.o dataContainer.o chromosome.o chromosomeTL.o chromosomePL.o rand.o population.o main.cpp
	$(CC) $(KONF) task.o dataContainer.o chromosome.o chromosomeTL.o chromosomePL.o rand.o population.o main.cpp -o $(MAIN)

task.o: task.h task.cpp
	$(CC) $(KONF) $(FLAG) task.h task.cpp

dataContainer.o: dataContainer.cpp dataContainer.h
	$(CC) $(KONF) $(FLAG) dataContainer.h dataContainer.cpp

chromosome.o: chromosome.cpp chromosome.h
	$(CC) $(KONF) $(FLAG) chromosome.h chromosome.cpp

chromosomeTL.o: chromosomeTL.cpp chromosomeTL.h
	$(CC) $(KONF) $(FLAG) chromosomeTL.h chromosomeTL.cpp

chromosomePL.o: chromosomePL.cpp chromosomePL.h
	$(CC) $(KONF) $(FLAG) chromosomePL.h chromosomePL.cpp

rand.o: rand.cpp rand.h
	$(CC) $(KONF) $(FLAG) rand.h rand.cpp

population.o: population.cpp population.h
	$(CC) $(KONF) $(FLAG) population.h population.cpp

clean:
	rm -f $(MAIN) *.o *.gch
