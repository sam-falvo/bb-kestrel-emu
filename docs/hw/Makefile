DOC = reference

reference.pdf: foo.pdf
	cp foo.pdf reference.pdf

foo.pdf: foo.dvi
	dvipdf foo

foo.dvi: foo.latex
	latex foo.latex
	latex foo.latex

foo.latex: reference.nw
	noweave -latex $(DOC).nw >foo.latex

clean:
	rm foo.*

