x = scan("ProductPrices.txt")
y = scan("PortfolioPrices.txt")
t = scan("time.txt")
plot(t,x,type="l",col="blue")
dev.new()
plot(t,y,type="l",col="red")
legend( x="topleft", 
         legend=c("Prix du produit","Prix du portefeuille de couverture"),
         col=c("blue","red"), pch = 20 , bty = "n" )
