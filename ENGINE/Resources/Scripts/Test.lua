local Hero = { }
Hero.__index = Hero
 
-- Constructor
function Hero.new ()
   local self = setmetatable({ },Hero)
   
   return self
end

function Hero:Ability1() 
	print("Ability1")
end

function Hero:Ability2() 
	print("Ability2")
end

function Hero:Ability3() 
	print("Ability3")
end

return Hero
