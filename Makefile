CXXFLAGS = -O2 -Wall -lrt -funroll-loops

run: mergesort funnelsort fast-mergesort
	#@echo
	#@echo "Running mergesort..."
	#./run_mergesort
	@echo
	@echo "Running funnelsort..."
	./run_funnelsort
	@echo
	@echo "Running fast-mergesort..."
	./run_fast-mergesort

plot:
	gnuplot sort.plot


mtxtrans: mtxtrans.cpp
	g++ $< -o run_$@ $(CXXFLAGS)

mtxtrans_loop: mtxtrans_loop.cpp
	g++ $< -o run_$@ $(CXXFLAGS)

SORT_OPTS = -DVERIFY_RESULT -DN=65536*32 -DNMIN=256 -DNMAX=16777216
MERGE_SORT_OPTS      = $(SORT_OPTS) -DTH=128
FUNNEL_SORT_OPTS     = $(SORT_OPTS) -DTH=128 -DBUFSIZE=TH
FAST_MERGE_SORT_OPTS = $(SORT_OPTS) -DTH=128

mergesort: mergesort.cpp
	g++ $< -o run_$@ $(CXXFLAGS) $(MERGE_SORT_OPTS)

funnelsort: funnelsort.cpp
	g++ $< -o run_$@ $(CXXFLAGS) $(FUNNEL_SORT_OPTS)

fast-mergesort: fast-mergesort.cpp
	g++ $< -o run_$@ $(CXXFLAGS) $(FUNNEL_SORT_OPTS)


clean:
	rm -f run_*
	rm -f *.ps
	rm -f *.eps
	rm -f *.dat
