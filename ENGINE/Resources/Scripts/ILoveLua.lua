local HeroModule = require("Resources/Scripts/Test")

local HeroObj = HeroModule.new()
HeroObj:Ability1()
HeroObj:Ability2()
HeroObj:Ability3()

-- Call C++ code :o
avg, sum = average(10, 20, 30, 40, 50)
print("The average is ", avg)
print("The sum is ", sum)










