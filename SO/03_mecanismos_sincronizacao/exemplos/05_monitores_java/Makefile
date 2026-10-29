all: MonitorBuffer.class
MonitorBuffer.class: MonitorBuffer.java
	javac $<
run: all
	java MonitorBuffer
clean:
	rm -f *.class
.PHONY: all run clean
