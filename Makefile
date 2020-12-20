PANDOC=/usr/bin/pandoc

.phony: all

all: c++.html python.html julia.html

%.html: %.md
	$(PANDOC) \
		-f markdown+tex_math_single_backslash \
		-t revealjs \
		--katex \
		-s \
		-V revealjs-url=https://ziotom78.github.io/tnds_final_seminars/reveal.js-4.1.0 \
		-V theme=white \
		-V width=1440 \
		-V height=810 \
		-o $@ \
		$<
