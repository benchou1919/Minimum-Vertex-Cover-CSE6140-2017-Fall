SYSTEM = x86-64_linux
LIBFORMAT = static_pic

CC = g++

# FOR BnB algorithm
CPLEXDIR = /opt/ibm/ILOG/CPLEX_Studio1263/cplex
CONCERTDIR = /opt/ibm/ILOG/CPLEX_Studio1263/concert

COPT =  -m64 -O -fPIC -fexceptions -DIL_STD -DNDEBUG
# COPT_NOCPLEX = -m64 -O -fPIC -fexceptions -DIL_STD -DNDEBUG -std=c++11
COPT_NOCPLEX = -m64 -O -fPIC -fexceptions -DIL_STD -DNDEBUG -std=c++11
CFLAGS = $(COPT) -I$(CPLEXDIR)/include -I$(CONCERTDIR)/include
LFLAGS = -L$(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT) -lilocplex -lcplex -L$(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT) -lconcert -lm -lpthread

OBJS = RunMVC.o Graph.o LS1.o LS2.o Approx.o BnB.o

# BNBSRC = RunMVC.cpp Graph.cpp BnB.cpp
# BNBOBJ = RunMVC.o Graph.o BnB.o LS2.o


# FOR LS algorithm
# LSSRC = RunMVC.cpp Graph.cpp
# LS1OBJ = RunMVC.o Graph.o LS1.o LS2.o Approx.o
# LS2OBJ = RunMVC.o Graph.o LS1.o LS2.o

# For Approx algorithm
# ApproxSRC = RunMVC.cpp Graph.cpp Approx.cpp
# ApproxOBJ = RunMVC.o Graph.o Approx.o

# OBJDIR = obj



# bnb: $(BNBOBJ)
# 	$(CC) $(COPT_NOCPLEX) $(BNBOBJ) -o RunMVC.BnB
# 	mv *.o $(OBJDIR)

BnB.o: BnB.cpp BnB.h Graph.h
	$(CC) -c $(COPT_NOCPLEX) BnB.cpp -o BnB.o

RunMVC.o: RunMVC.h RunMVC.cpp LS1.h LS2.h Approx.cpp Approx.h BnB.h
	$(CC) -c $(COPT_NOCPLEX) RunMVC.cpp

Graph.o: Graph.h Graph.cpp
	$(CC) -c $(COPT_NOCPLEX) Graph.cpp

LS2.o: LS2.h LS2.cpp Graph.h
	$(CC) -c $(COPT_NOCPLEX) LS2.cpp

LS1.o: LS1.h LS1.cpp Graph.h
	$(CC) -c $(COPT_NOCPLEX) LS1.cpp

# LS: $(LS1OBJ)
# 	$(CC) ${COPT_NOCPLEX} $(LS1OBJ) -o exec
#
# LS2: $(LS2OBJ)
# 	$(CC) ${COPT_NOCPLEX} $(LS2OBJ) -o exec

Approx.o: Approx.h Approx.cpp Graph.h
	$(CC) -c $(COPT_NOCPLEX) Approx.cpp

# Approx: $(ApproxOBJ)
# 	$(CC) ${COPT_NOCPLEX} $(ApproxOBJ) -o Approx.out

proj: $(OBJS)
	$(CC) ${COPT_NOCPLEX} $(OBJS) -o exec

run:
	./exec -inst ../Data/karate.graph -alg Approx -seed -1
	# for number in 0 1 2 3 4 5 6 7 8 9; do \
	#
	# 	./exec -inst ../Data/karate.graph -alg LS2 -time 600 -seed $$number; \
    # done
	# ./exec -inst ../Data/karate.graph -alg LS1 -time 600 -seed $$number; \
	# ./exec -inst ../Data/karate.graph -alg LS2 -time 600 -seed $$number; \
	# ./exec -inst ../Data/karate.graph -alg BnB -time 600 -seed $$number; \

	# ./exec -inst ../Data/football.graph -alg LS1 -time 600 -seed $$number; \
	# ./exec -inst ../Data/netscience.graph -alg LS1 -time 600 -seed $$number; \
	# ./exec -inst ../Data/as-22july06.graph -alg LS1 -time 55 -seed $$number; \
	# ./exec -inst ../Data/hep-th.graph -alg LS1 -time 80 -seed $$number; \
	# ./exec -inst ../Data/star.graph -alg LS1 -time 130 -seed $$number; \
	# ./exec -inst ../Data/star2.graph -alg LS1 -time 80 -seed $$number; \
	# ./exec -inst ../Data/jazz.graph -alg LS1 -time 2 -seed $$number; \
	# ./exec -inst ../Data/email.graph -alg LS1 -time 40 -seed $$number; \
	# ./exec -inst ../Data/delaunay_n10.graph -alg LS1 -time 45 -seed $$number; \
	# ./exec -inst ../Data/power.graph -alg LS1 -time 60 -seed $$number; \
	# ./exec -inst ../Data/jazz.graph -alg LS2 -time 600 -seed $$number; \
	# ./exec -inst ../Data/karate.graph -alg LS2 -time 600 -seed $$number; \
	# ./exec -inst ../Data/football.graph -alg LS2 -time 600 -seed $$number; \
	# ./exec -inst ../Data/as-22july06.graph -alg LS2 -time 600 -seed $$number; \
	# ./exec -inst ../Data/hep-th.graph -alg LS2 -time 600 -seed $$number; \
	# ./exec -inst ../Data/star.graph -alg LS2 -time 600 -seed $$number; \
	# ./exec -inst ../Data/star2.graph -alg LS2 -time 600 -seed $$number; \
	# ./exec -inst ../Data/netscience.graph -alg LS2 -time 600 -seed $$number; \
	# ./exec -inst ../Data/email.graph -alg LS2 -time 600 -seed $$number; \
	# ./exec -inst ../Data/delaunay_n10.graph -alg LS2 -time 600 -seed $$number; \
	# ./exec -inst ../Data/power.graph -alg LS2 -time 600 -seed $$number; \
	# for number in 0 1 2 3 4 5 6 7 8 9; do \


clean:
	rm -f *.o
	rm exec
