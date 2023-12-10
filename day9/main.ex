defmodule Solution do

  def parse(line) do
    line = line
      |> String.split(" ", trim: true)
      |> Enum.map( fn str ->
          {d, _} = Integer.parse(str)
          d
      end)
    [line]
  end


  def diffs(nums) do
    last = List.last(nums)
    if Enum.all?(last, &( &1 == 0)) do
      List.delete_at(nums, -1)
    else
      diff = last
        |> Enum.reduce([0], fn (n, acc) ->
              d = n - List.last(acc)
              acc = List.replace_at(acc, -1, d)
              acc ++ [n]
            end)
      diff = diff
        |> List.delete_at(-1) 
        |> List.delete_at(0)
      diffs(nums ++ [diff])
    end
  end

  def post_extrapolate(nums) do
    List.foldr(nums, 0 , fn (l, acc) -> 
      l |> List.last() |> then fn(n) -> n + acc end
    end)
  end
  def pre_extrapolate(nums) do
    List.foldr(nums, 0 , fn (l, acc) -> 
      l |> List.first() |> then fn(n) -> n - acc end
    end)

  end
  
  def part1(c) do
    c |> String.split("\n", trim: true)
      |> Enum.map( fn line ->
          line
            |> parse()
            |> diffs()
            |> post_extrapolate()
        end)
      |> Enum.sum()
      |> IO.inspect()

  end
  
  def part2(c) do
    c |> String.split("\n", trim: true)
      |> Enum.map( fn line ->
          line
            |> parse()
            |> diffs()
            |> pre_extrapolate()
        end)
      |> Enum.sum()
      |> IO.inspect()

  end
end

# case File.read("example.txt") do
case File.read("puzzle.txt") do
  {:ok, content} ->
    Solution.part1(content)
    Solution.part2(content)
  {:error} -> IO.put("Failed to read example.txt")
end
