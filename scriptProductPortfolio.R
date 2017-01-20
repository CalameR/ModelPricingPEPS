x = scan("ProductPrices.txt")
y = scan("PortfolioPrices.txt")
t = scan("time.txt")
plot(t,x,type="l",col="blue")
lines(t,y,col="red")

