defmodule Solution do
  defp hash(s), do: hash(s, 0)
  defp hash([], v), do: v
  defp hash([c | s], v) do
    v = v + c
    v = v * 17
    v = rem(v, 256)
    hash(s, v)
  end

  defp hash_char(c, v) do
    v = v + c
    v = v * 17
    v = rem(v, 256)
    v
  end

  def part1(c) do
    c |> String.replace("\n", "")
      |> String.split(",", trim: true)
      |> Enum.map(&to_charlist/1)
      |> Enum.map( fn b -> 
          Enum.reduce(b, 0, &hash_char/2)
          end)
      |> Enum.sum()
      |> IO.inspect
  end

  def ins({b_id, :set, l_id, r}, boxes) do
    Map.update!(boxes, b_id, fn lenses -> 
      if Enum.any?(lenses, fn {l_ido, _} -> l_ido == l_id end) do
        Enum.map(lenses, fn {l_ido, ro} ->
          if l_ido == l_id do
            {l_ido, r}
          else
            {l_ido, ro}
          end
        end)
      else
        lenses ++ [{l_id, r}]
      end
    end)
  end
  def ins({b_id, :sub, l_id}, boxes) do
    Map.update!(boxes, b_id, fn lenses -> 
      if Enum.any?(lenses, fn {l_ido, _} -> l_ido == l_id end) do
        Enum.reduce(lenses, [], fn ({l_ido, ro}, acc) ->
          if l_ido == l_id do
            acc
          else
            acc ++ [{l_ido, ro}]
          end
        end)
      else
        lenses
      end
    end)
  end

  def part2(c) do
    boxes = Enum.reduce( 0..255, %{}, &Map.put(&2, &1, []) )
    c |> String.replace("\n", "")
      |> String.split(",", trim: true)
      |> Enum.map( fn i ->
          i |> to_charlist()
            |> Enum.chunk_by(&( &1 in [61, 45]))
            |> then( fn [id, op | r] ->
                op = if op == ~c"=" do :set else :sub end
                r = if r != [] do List.first(List.first(r)) - 0x30 else [] end
                if op == :set do
                  {hash(id), op, to_string(id), r}
                else
                  {hash(id), op, to_string(id)}
                end
            end)
      end)
      |> Enum.reduce(boxes, fn (i, acc) -> ins(i, acc) end)
      |> Map.filter( fn{_, v} -> v != [] end)
      |> Enum.to_list()
      |> Enum.map( fn {b, l} -> 
          Enum.map(Enum.with_index(l), fn {{_, a}, i} -> {a, b+1, i+1} end) 
          end)
      |> List.flatten()
      |> Enum.map( &Tuple.product/1 )
      |> Enum.sum()
      |> IO.inspect()
  end
end

case File.read("example.txt") do
# case File.read("puzzle.txt") do
  {:ok, content} ->
    Solution.part1(content)
    Solution.part2(content)
  {:error} -> IO.put("Failed to read example.txt")
end
