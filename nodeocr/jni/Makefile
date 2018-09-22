all: ../libs/x86/nodeocr.node ../libs/x86_64/nodeocr.node

../libs/x86/nodeocr.node: nodeocr.cc
	mkdir -p ../libs/x86
	/opt/android-ndk-x86/bin/clang++ -o $@ $^ -I../../node/x86/include/node -L../../msocrw/libs/x86 -lmsocrw -fPIC -shared

../libs/x86_64/nodeocr.node: nodeocr.cc
	mkdir -p ../libs/x86_64
	/opt/android-ndk-x86_64/bin/clang++ -o $@ $^ -I../../node/x86_64/include/node -L../../msocrw/libs/x86_64 -lmsocrw -fPIC -shared

clean:
	rm -f ../libs/x86/nodeocr.node ../libs/x86_64/nodeocr.node