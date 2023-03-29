file = open ("final_result.out","r")
data = file.read()
n = data.count("Looks good!")
print('Successful runs: ', n)
