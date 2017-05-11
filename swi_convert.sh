# replace any swi instructions with a jump to the 1up/dummy hooks
sed -i '' -e "/^[ \t]*swi/ s/swi/mov\tr14, pc\nmov\tpc, #/g" homebrew.s
