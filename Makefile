.PHONY: clean All

All:
	@echo "----------Building project:[ 545_5 - Debug ]----------"
	@cd "545_5" && "$(MAKE)" -f  "545_5.mk"
clean:
	@echo "----------Cleaning project:[ 545_5 - Debug ]----------"
	@cd "545_5" && "$(MAKE)" -f  "545_5.mk" clean
